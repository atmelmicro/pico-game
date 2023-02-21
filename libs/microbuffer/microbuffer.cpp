#include "microbuffer.h"

typedef struct {
    uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
    uint8_t width;         ///< Bitmap dimensions in pixels
    uint8_t height;        ///< Bitmap dimensions in pixels
    uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
    int8_t xOffset;        ///< X dist from cursor pos to UL corner
    int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
    uint8_t *bitmap;     ///< Glyph bitmaps, concatenated
    GFXglyph *glyph;     ///< Glyph array
    uint16_t first;      ///< ASCII extents (first char)
    uint16_t last;       ///< ASCII extents (last char)
    uint8_t yAdvance;    ///< Newline distance (y axis)
    const char *subset;  ///< subset of chars in the font
} GFXfont;

GFXfont *_gfxFont;
void setFont(const GFXfont *f) {
    _gfxFont = (GFXfont *)f;
}

#define LCD_ASCII_OFFSET 0x20

int Framebuffer::get_pixel_pos(int x, int y) {
    return (y * width) + x;
}
Framebuffer::Framebuffer(int fb_width, int fb_height) {
    width = fb_width;
    height = fb_height;
    length = fb_width * fb_height;
    buffer = new uint16_t[length];
}
void Framebuffer::fill(Color color) {
    std::fill_n(buffer, length, color.rgb556);
}
void Framebuffer::debug_print() {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if(buffer[col + (row * width)] != 0) {
                std::cout << "# ";
            } else std::cout << ". ";
        }
        std::cout << std::endl;
    }
}
void Framebuffer::draw_h_line(int x, int y, int w, Color color) {
    if((y * width) + x > length) { return; }
    std::fill_n(buffer + (y * width) + x, w, color.rgb556);
}
void Framebuffer::draw_v_line(int x, int y, int h, Color color) {
    if((y * width) + x > length) { return; }
    for (int i = 0; i < h; ++i) {
        buffer[((y + i) * width) + x] = color.rgb556;
    }
}
void Framebuffer::draw_rect(int x, int y, int w, int h, Color color) {
    // top and bottom
    draw_h_line(x, y, w - 1, color);
    draw_h_line(x, y + h - 1, w, color);

    // left and right
    draw_v_line(x, y, h - 1, color);
    draw_v_line(x + w - 1, y, h - 1, color);
}

void Framebuffer::draw_fill_rect(int x, int y, int w, int h, Color color) {
    for (int i = y; i < y + h; ++i) {
        draw_h_line(x, i, w, color);
    }
}

void Framebuffer::draw_pixel(int x, int y, Color color) { // add color
    buffer[get_pixel_pos(x, y)] = color.rgb556;
}

void Framebuffer::draw_char(int x, int y, Color color, char ch) {
    for (int i = 0; i < 5; ++i) {
        uint8_t line;
        line = Font[(ch - LCD_ASCII_OFFSET) * 5 + i];
        for (int j = 0; j < 7; ++j, line >>= 1) {
            // y + j - flipped counterclockwise
            if(line & 0x01) draw_pixel(x + i, y + j, color);
            else std::cout << ".";
        }
        std::cout << std::endl;
    }
}

void Framebuffer::draw_string(int x, int y, Color color, const std::string& string) {
    for (int i = 0; i < string.length(); ++i) {
        draw_char(x + (i * (5 /* char width */ + 1)), y, color, string[i]);
    }
}

void draw_circle(int x, int y, int r, Color color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    draw_pixel(x0  , y0 + r, color);
    draw_pixel(x0  , y0 - r, color);
    draw_pixel(x0+r, y0    , color);
    draw_pixel(x0-r, y0    , color);
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        draw_pixel(x0 + x, y0 + y, color);
        draw_pixel(x0 - x, y0 + y, color);
        draw_pixel(x0 + x, y0 - y, color);
        draw_pixel(x0 - x, y0 - y, color);
        draw_pixel(x0 + y, y0 + x, color);
        draw_pixel(x0 - y, y0 + x, color);
        draw_pixel(x0 + y, y0 - x, color);
        draw_pixel(x0 - y, y0 - x, color);
    }
}

void void draw_fill_circle(int x, int y, int r, Color color) {
    draw_v_line(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

Color::Color(int r, int b, int g) {
    this->rgb556 = ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

Color::Color() = default;