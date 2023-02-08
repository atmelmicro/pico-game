//
// Created by David on 23.11.2022.
//
#include <vector>
#include <map>
#include "microbuffer.h"
#include "bmp.h"
#include <memory>

#ifndef LCD_S_LIB_H
#define LCD_S_LIB_H

class SObject {
public:
    int16_t y_points[3], x_points[3];
    Color color;

    virtual void Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {}
    void Move(int16_t new_x, int16_t new_y);
    SObject() {};
};


class Screen {
public:
    std::map<std::string, std::unique_ptr<SObject> > SObjects;
    std::shared_ptr<Framebuffer> fb;
    void Draw();
    int Remove_SObject(const std::string& name);

    Screen();
};

class Rect : public SObject {
public:
    Rect(int16_t x, int16_t y, int16_t w, int16_t h, Color color);
    void Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) override;
};

class Fill_Rect : public SObject {
public:
    Fill_Rect(int16_t x, int16_t y, int16_t w, int16_t h, Color color);
    void Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) override;
};

class Pixel : public SObject {
public:
    Pixel(int16_t x, int16_t y, Color color);
    void Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) override;
};

class Text : public SObject {
private:
    std::basic_string<char> str;
public:
    Text(int16_t x, int16_t y, Color color, std::string string);
    void Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) override;
};

class Image : public SObject {
private:
    BitmapFile *file;
public:
    Image(int16_t x, int16_t y, const std::string& file_name);
    void Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) override;
};

#endif //LCD_S_LIB_H
