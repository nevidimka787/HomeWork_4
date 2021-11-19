#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec4 segment; 

uniform float scale;
uniform vec2 camera_position;
uniform float camera_size;

#define CAMERA_MAX_SIZE 5.0f
#define CAMERA_MIN_SIZE 0.2f

mat3 Rotate(float angel);
mat3 Transport(vec2 vector);
mat3 Scale(vec2 vector);
mat3 SetBySegment(vec4 _segment);

mat3 matrix;

out vec2 pixel_position;
void main()
{
    pixel_position = aPos;
    matrix =
        Scale(vec2(1.0f, camera_size / max(CAMERA_MIN_SIZE, min(camera_size, CAMERA_MAX_SIZE)))) *
        SetBySegment(segment) *
        Transport(-camera_position) *
        Scale(vec2(1.0f / camera_size)) *
        Scale(vec2(1.0f, scale));
    
    gl_Position = vec4(vec3(aPos, 1.0f) * matrix, 1.0f);
}

mat3 Transport(vec2 vector)
{
    return mat3(
        1.0f,   0.0f,   vector.x, 
        0.0f,   1.0f,   vector.y,
        0.0f,   0.0f,   1.0f);
}


mat3 Scale(vec2 vector)
{
    return mat3(
        vector.x,   0.0f,       0.0f,
        0.0f,       vector.y,   0.0f,
        0.0f,       0.0f,       1.0f);
}

mat3 Rotate(float angle)
{
    return mat3(
        cos(angle),    -sin(angle),     0.0f,
        sin(angle),     cos(angle),     0.0f,
        0.0f,           0.0f,           1.0f);
}

mat3 SetBySegment(vec4 _segment)
{
    if(_segment.z >= 0.0f)
    {
        return Scale(vec2(length(_segment.zw), 1.0f)) * Rotate(atan(_segment.w / _segment.z)) * Transport(_segment.xy);
    }
    if(_segment.w > 0.0f)
    {
        return Scale(vec2(length(_segment.zw), 1.0f)) * Rotate(atan(_segment.w / _segment.z) + radians(180.0f)) * Transport(_segment.xy);
    }
    return Scale(vec2(length(_segment.zw), 1.0f)) * Rotate(atan(_segment.w / _segment.z) - radians(180.0f)) * Transport(_segment.xy);
}
