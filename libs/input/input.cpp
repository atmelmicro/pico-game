//
// Created by David on 28.02.2023.
//
#include "input.h"

#include <utility>

uint64_t time;
uint32_t delayTime;

void* internal_param;
std::function<void(void *)> internal_up_callback;
std::function<void(void *)> internal_down_callback;
std::function<void(void *)> internal_left_callback;
std::function<void(void *)> internal_right_callback;

void gpio_callback(uint gpio, uint32_t events) {
    if ((to_ms_since_boot(get_absolute_time()) - time) > delayTime) {
        time = to_ms_since_boot(get_absolute_time());
        switch (gpio) {
            case 1: // down
                internal_down_callback(internal_param);
                break;
            case 22: // right
                internal_right_callback(internal_param);
                break;
            case 21: // up
                internal_up_callback(internal_param);
                break;
            case 20: // left
                internal_left_callback(internal_param);
                break;
            default:
                return;
        }
    }
}

Input::Input(
        std::function<void(void *)> up_callback,
        std::function<void(void *)> down_callback,
        std::function<void(void *)> left_callback,
        std::function<void(void *)> right_callback,
        void *param,
        bool use_accel,
        int tick_freq
        ) {
    this->param = param;
    internal_up_callback = std::move(up_callback);
    internal_down_callback = std::move(down_callback);
    internal_left_callback = std::move(left_callback);
    internal_right_callback = std::move(right_callback);
    internal_param = param;
    delayTime = tick_freq;

    int buttons_gpios[] = {1, 22, 21, 20};

    for (int gpio : buttons_gpios) {
        gpio_pull_up(gpio);
        gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_LEVEL_LOW, true, gpio_callback);
    }
}