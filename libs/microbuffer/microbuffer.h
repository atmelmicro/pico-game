//
// Created by David on 17.01.2023.
//

#ifndef LCD_MICROBUFFER_H
#define LCD_MICROBUFFER_H
#include <iostream>
#include "TextFonts.h"

class Color {
public:
    Color();
    Color(int r, int b, int g);

    uint16_t rgb556;
};

class Framebuffer {
private:
    int length, width, height;
    int get_pixel_pos(int x, int y);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, Color color);
public:
    uint16_t *buffer;
    Framebuffer(int fb_width, int fb_height);
    void fill(Color color);
    void debug_print();
    void draw_h_line(int x, int y, int w, Color color);
    void draw_v_line(int x, int y, int h, Color color);
    void draw_rect(int x, int y, int w, int h, Color color);
    void draw_fill_rect(int x, int y, int w, int h, Color color);
    void draw_pixel(int x, int y, Color color);
    void draw_char(int x, int y, Color color, char ch);
    void draw_string(int x, int y, Color color, const std::string& string);
    void draw_circle(int x0, int y0, int r, Color color);
    void draw_fill_circle(int x0, int y0, int r, Color color);
};

#endif //LCD_MICROBUFFER_H
