//
// Created by David on 28.02.2023.
//

#ifndef LCD_INPUT_H
#define LCD_INPUT_H
#include <functional>
#include "hardware/gpio.h"
#include "pico/time.h"

enum button_dir {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Input {
private:
    int tick_freq;
    bool use_accel;
    void* param;
public:
    Input(
            std::function<void(void*)> up_callback,
            std::function<void(void*)> down_callback,
            std::function<void(void*)> left_callback,
            std::function<void(void*)> right_callback,
            void* param,
            bool use_accel = false,
            int tick_freq = 100
                    );
};

#endif //LCD_INPUT_H
