//
// Created by David on 23.11.2022.
//
#include "hw.h"
#include "ST7735_TFT.h"
#include "s_lib.h"
#include <array>

void draw_buffer(uint16_t *buffer) {
    setAddrWindow(0, 0, 128 - 1, 160 - 1);
    std::vector<uint8_t> buffer_8b;
    for (int i = 0; i < 128 * 160; ++i) {
        uint16_t e = buffer[i];
        buffer_8b.push_back(e >> 8);
        buffer_8b.push_back(e);
    }
    tft_dc_high();
    tft_cs_low();
    spi_write_blocking(SPI_TFT_PORT, &buffer_8b[0], buffer_8b.size());
    tft_cs_high();
}

Rect::Rect (int16_t x, int16_t y, int16_t w, int16_t h, Color color) {
    this->x_points[0] = x;
    this->x_points[1] = w;
    this->y_points[0] = y;
    this->y_points[1] = h;
    this->color = color;
}

void Rect::Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {
    parentFramebuffer->draw_rect(this->x_points[0], this->y_points[0], this->x_points[1], this->y_points[1], this->color);
}

Fill_Rect::Fill_Rect (int16_t x, int16_t y, int16_t w, int16_t h, Color color) {
    this->x_points[0] = x;
    this->x_points[1] = w;
    this->y_points[0] = y;
    this->y_points[1] = h;
    this->color = color;
}

void Fill_Rect::Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {
    parentFramebuffer->draw_fill_rect(this->x_points[0], this->y_points[0], this->x_points[1], this->y_points[1], this->color);
}

void SObject::Move(int16_t new_x, int16_t new_y) {
    this->x_points[0] = new_x;
    this->y_points[0] = new_y;
}

Pixel::Pixel(int16_t x, int16_t y, Color color) {
    this->x_points[0] = x;
    this->y_points[0] = y;
    this->color = color;
}
void Pixel::Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {
    parentFramebuffer->draw_pixel(this->x_points[0], this->y_points[0], this->color);

}

Text::Text(int16_t x, int16_t y, Color color, std::string string) {
    this->x_points[0] = x;
    this->y_points[0] = y;
    this->color = color;
    this->str = string;
}
void Text::Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {
    parentFramebuffer->draw_string(this->x_points[0], this->y_points[0], this->color, this->str);
}

Image::Image(int16_t x, int16_t y, const std::string& file_name) {
    this->file = new BitmapFile(file_name.c_str());
    this->x_points[0] = x;
    this->y_points[0] = y;
}

void Image::Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {
    this->file->Draw(parentFramebuffer, this->x_points[0], this->y_points[0]);
}

Circle::Circle(int16_t x, int16_t y, int16_t r,Color color) {
    this->x_points[0] = x;
    this->y_points[0] = y;
    this->x_points[1] = r;
    this->color = color;
}
void Circle::Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {
    parentFramebuffer->draw_circle(this->x_points[0], this->y_points[0], this->x_points[1], this->color);
}

Fill_Circle::Fill_Circle(int16_t x, int16_t y, int16_t r,Color color) {
    this->x_points[0] = x;
    this->y_points[0] = y;
    this->x_points[1] = r;
    this->color = color;
}
void Fill_Circle::Draw(const std::shared_ptr<Framebuffer>& parentFramebuffer) {
    parentFramebuffer->draw_fill_circle(this->x_points[0], this->y_points[0], this->x_points[1], this->color);
}


void Screen::Draw() {
    fb->fill(Color(0, 0, 0));
    for(const auto& [key, value] : SObjects) {
        value->Draw(this->fb);
    }
    draw_buffer(fb->buffer);
}

Screen::Screen() {
    spi_init(spi0, 133 * 1000000);         // SPI with 133 max
    gpio_set_function(16, GPIO_FUNC_SPI); // TODO: make this a var
    gpio_set_function(19, GPIO_FUNC_SPI);
    gpio_set_function(18, GPIO_FUNC_SPI);
    tft_spi_init();

    TFT_BlackTab_Initialize();
    setTextWrap(true);
    sleep_ms(100);
    fillScreen(ST7735_BLACK);
    fb = std::make_shared<Framebuffer>(128, 160);
}