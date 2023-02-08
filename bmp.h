//
// Created by David on 17.01.2023.
//

#ifndef LCD_BMP_H
#define LCD_BMP_H

#include <iostream>
#include "pico_hal.h"
#include "microbuffer.h"
#include <memory>

class BitmapFile {
private:
    uint8_t *buffer;
    int height, width;
public:
    BitmapFile(const char *file_name);
    void Draw(const std::shared_ptr<Framebuffer>& fb, int x, int y);
};

#endif //LCD_BMP_H
