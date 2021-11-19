#version 330 core
layout (location = 0) in vec2 aPos;

uniform float radius;
uniform vec2 position;

uniform float scale;
uniform vec2 camera_position;
uniform float camera_size;

mat3 Rotate(float angel);
mat3 Transport(vec2 vector);
mat3 Scale(vec2 vector);

mat3 matrix;

#define CAMERA_MAX_SIZE 8.0f
#define CAMERA_MIN_SIZE 0.5f

out vec2 pixel_position;
void main()
{
    pixel_position = aPos;

    matrix = 
        Scale(vec2(camera_size / max(CAMERA_MIN_SIZE, min(camera_size, CAMERA_MAX_SIZE)))) *
        Scale(vec2(radius)) * 
        Transport(position) * 
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
