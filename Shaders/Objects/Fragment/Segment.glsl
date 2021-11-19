#version 330 core

in vec2 pixel_position;

out vec4 fragment_color;

uniform float rounded;

float Length(vec2 vec, float e);

void main()
{
    if(abs(pixel_position.y) > 0.01f)
    {
       discard;
    }
    
    fragment_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
