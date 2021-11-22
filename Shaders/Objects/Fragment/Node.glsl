#version 330 core

in vec2 pixel_position;

out vec4 fragment_color;

uniform sampler2D symbols_texture;

uniform int text[32];
uniform int text_length;
uniform float text_size;
uniform int color_id;

#define TEXT_SIZE_100   100.0f
#define TEXTURE_CELL_SIZE (1.0f / 16.0f)

#define TEXT_SCALE 0.6f

#define TRUE  1
#define FALSE 0
#define ERROR -1

#define COLOR_BLACK vec3(0.0f)
#define COLOR_WHITE vec3(1.0f)

#define BORD_COLOR COLOR_BLACK

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
    
    vec3 bg_color;
    
    int new_color_id_r = 
        ((color_id & (1 <<  0)) >>  0) +
        ((color_id & (1 <<  3)) >>  2) +
        ((color_id & (1 <<  6)) >>  4) +
        ((color_id & (1 <<  9)) >>  6) +
        ((color_id & (1 << 12)) >>  8) +
        ((color_id & (1 << 15)) >> 10) +
        ((color_id & (1 << 18)) >> 12) +
        ((color_id & (1 << 21)) >> 14) +
        ((color_id & (1 << 24)) >> 16) +
        ((color_id & (1 << 27)) >> 18) +
        ((color_id & (1 << 30)) >> 20);
        
    int new_color_id_g = 
        ((color_id & (1 <<  1)) >>  1) +
        ((color_id & (1 <<  4)) >>  3) +
        ((color_id & (1 <<  7)) >>  5) +
        ((color_id & (1 << 10)) >>  7) +
        ((color_id & (1 << 13)) >>  0) +
        ((color_id & (1 << 16)) >> 11) +
        ((color_id & (1 << 19)) >> 13) +
        ((color_id & (1 << 22)) >> 15) +
        ((color_id & (1 << 25)) >> 17) +
        ((color_id & (1 << 28)) >> 19) +
        ((color_id & (1 << 31)) >> 21);
        
    int new_color_id_b = 
        ((color_id & (1 <<  2)) >>  2) +
        ((color_id & (1 <<  5)) >>  4) +
        ((color_id & (1 <<  8)) >>  6) +
        ((color_id & (1 << 11)) >>  8) +
        ((color_id & (1 << 14)) >> 10) +
        ((color_id & (1 << 17)) >> 12) +
        ((color_id & (1 << 20)) >> 14) +
        ((color_id & (1 << 23)) >> 16) +
        ((color_id & (1 << 26)) >> 18) +
        ((color_id & (1 << 29)) >> 20);
        
    if(color_id == 0)
    {
        bg_color = vec3(1.0f);
    }
    else
    {
        #define POW 4.0f
        float mult = max(max(pow(float(new_color_id_r), POW), pow(float(new_color_id_g), POW)), pow(float(new_color_id_b), POW));
        bg_color = vec3(float(new_color_id_r), float(new_color_id_g), float(new_color_id_b));
        bg_color = 1.0f - bg_color / mult;
        
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
                    if(length(bg_color) < 0.5f)
                    {
                        fragment_color = vec4(COLOR_WHITE, 1.0f);
                    }
                    else
                    {
                        fragment_color = vec4(COLOR_BLACK, 1.0f);
                    }
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
        fragment_color = vec4(bg_color, 1.0f);
        return;
    }
    
    fragment_color = vec4(BORD_COLOR, 1.0f);
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


































