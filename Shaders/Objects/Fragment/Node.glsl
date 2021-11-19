#version 330 core

in vec2 pixel_position;

out vec4 fragment_color;

uniform sampler2D symbols_texture;

uniform int text[32];
uniform int text_length;
uniform float text_size;

#define TEXT_SIZE_100   100.0f
#define TEXTURE_CELL_SIZE (1.0f / 16.0f)

#define TEXT_SCALE 0.6f

#define TRUE  1
#define FALSE 0
#define ERROR -1

#define COLOR_WHILE vec3(1.0f)
#define COLOR_BLACK vec3(0.0f)

#define INSIDE_RADIUS   0.95f

vec2 ConvertPosition(vec2 _pos, vec4 _from_cell, vec4 _to_cell);
int PixelInSymbolFrame(vec2 _pix_pos, vec4 _frame);
vec4 SymbolFrame(vec2 _frame, int _symbol_numer, int _text_length);
vec2 TextFrame(int _text_length, float _text_size);
vec2 TexturePosition(vec2 _pix_pos, vec4 cell, int _symbol);

void main()
{
    if(length(pixel_position) > 1.0f)
    {
        discard;
    }
    
    vec2 frame = TextFrame(text_length, text_size);
    if(abs(pixel_position.x) < frame.x && abs(pixel_position.y) < frame.y)
    {
        vec4 cell;
        for(int s = 0; s < text_length; s++)
        {
            cell = SymbolFrame(frame, s, text_length);
            if(PixelInSymbolFrame(pixel_position, cell) == TRUE)
            {
                if(texture(
                    symbols_texture,
                    TexturePosition(pixel_position, cell, text[s])
                    ).z > 0.9f)
                {
                    fragment_color = vec4(COLOR_BLACK, 1.0f);
                    return;
                }
                else
                {
                    break;
                }
                
            }
        }
    }
    
    if(length(pixel_position) < INSIDE_RADIUS)
    {
        fragment_color = vec4(COLOR_WHILE, 1.0f);
        return;
    }
    
    fragment_color = vec4(COLOR_BLACK, 1.0f);
}

vec2 TextFrame(int _text_length, float _text_size)
{
    float size = _text_size / TEXT_SIZE_100;
    
    return vec2(size * _text_length * TEXT_SCALE, size) / 2.0f;
}

vec4 SymbolFrame(vec2 _frame, int _symbol_numer, int _text_length)
{
    return vec4(
        -_frame.x + (2.0f * _frame.y * _symbol_numer) * TEXT_SCALE,
        -_frame.y,
        -_frame.x + (2.0f * _frame.y * (_symbol_numer + 1)) * TEXT_SCALE,
        _frame.y);
}

int PixelInSymbolFrame(vec2 _pix_pos, vec4 _frame)
{
    if(_pix_pos.x > _frame.x && _pix_pos.x < _frame.z && _pix_pos.y > _frame.y && _pix_pos.y < _frame.w)
    {
        return 1;
    }
    return 0;
}

vec2 ConvertPosition(vec2 _pos, vec4 _from_cell, vec4 _to_cell)
{
    vec2 loc_pos = (_pos - _from_cell.xy) / (_from_cell.zw - _from_cell.xy);
    return loc_pos * (_to_cell.zw - _to_cell.xy) + _to_cell.xy;
}

vec2 TexturePosition(vec2 _pix_pos, vec4 _cell, int _symbol)
{
    vec4 texture_cell_frame = vec4(
        (_symbol % 16) * TEXTURE_CELL_SIZE + TEXTURE_CELL_SIZE * 0.1f,
        (_symbol / 16) * TEXTURE_CELL_SIZE + TEXTURE_CELL_SIZE,
        (_symbol % 16) * TEXTURE_CELL_SIZE + TEXTURE_CELL_SIZE - TEXTURE_CELL_SIZE * 0.1f,
        (_symbol / 16) * TEXTURE_CELL_SIZE);
    
    return ConvertPosition(_pix_pos, _cell, texture_cell_frame);
}


































