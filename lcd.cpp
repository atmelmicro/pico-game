//
// Created by David on 23.11.2022.
//
#include <memory>

#include "pico/stdlib.h"
#include "s_lib.h"
#include "ST7735_TFT.h"
#include "hardware/spi.h"
#include "bmp.h"

int main() {
    stdio_init_all();
    sleep_ms(400);
    Screen screen;
    sleep_ms(400);

    if (pico_mount(false) != LFS_ERR_OK) {
        printf("Error mounting FS\n");
        exit(-1);
    }

    Rect *a = new Rect(10, 0, 10, 10, Color(0, 0, 255));
    screen.SObjects["a"].reset(a);
    screen.SObjects["hw_text"] = std::make_unique<Text>(0, 0, Color(0, 255, 0), "Hello World!");
    screen.SObjects["pixel"] = std::make_unique<Pixel>(58, 100, Color(255, 0, 0));
    screen.Draw();
    sleep_ms(2000);

    for (int16_t i = 0; i < 140; ++i) {
        screen.SObjects["a"]->Move(30, 10); // change this maybe?
        screen.Draw(); // lowercase
    }

    sleep_ms(4000);

    while(true) {};
    return 0;
}
