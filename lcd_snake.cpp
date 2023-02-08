#include "pico/stdlib.h"
#include "pico/time.h" // more efficient rand stuff maybe???
#include "pico/stdio.h"
#include "hw.h"
#include "ST7735_TFT.h"
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include <algorithm>

#define snakeOffset 0
#define shiftSnakeBy 10
#define snakeRefreshRate 600
void drawApple();
enum direction {
    UP_DIR = 0,
    DOWN_DIR = 1,
    LEFT_DIR = 2,
    RIGHT_DIR = 3
};

unsigned long time = to_ms_since_boot(get_absolute_time());
const int delayTime = 100; // Delay for every push button may vary
bool snakeAteApple = false;
int currentOffsetX = 0;
int currentOffsetY = 0;
direction currentDir = RIGHT_DIR;

std::vector<int> snakeX; // 0 - x, 1 -y
std::vector<int> snakeY; // 0 - x, 1 -y

std::vector<direction> dirs; // 0 - x, 1 -y

int appleLocation[] = {0, 0};

void drawSnake() {
    snakeX.push_back(currentOffsetX);
    snakeY.push_back(currentOffsetY);
    fillRect(currentOffsetX + snakeOffset + 4, // Move right
             currentOffsetY + snakeOffset + 5,
             10 - (snakeOffset * 2),
             10 - (snakeOffset * 2),
             ST7735_GREEN
    );

    fillRect(snakeX.front() + snakeOffset + 4, // Move right
             snakeY.front() + snakeOffset + 5,
             10 - (snakeOffset * 2),
             10 - (snakeOffset * 2),
             ST7735_BLACK
    );
    if(!snakeAteApple) {
        snakeX.erase(snakeX.begin());
        snakeY.erase(snakeY.begin());
    } else {snakeAteApple = false;}

}

bool repeating_timer_callback(struct repeating_timer *t) {
    /*if (currentOffset + 10 == 120) { // 120 w 150h
        return false;
    }*/
    if (0 < dirs.size() - 1) {
        if(dirs.size() > 2) {
            if(dirs[0] == dirs[1])  dirs.erase(dirs.begin());
        }
        dirs.erase(dirs.begin());
/*        direction dir = dirs[0];
        switch (dir) {
            case DOWN_DIR:
                currentOffsetY += 10;
                break;
            case UP_DIR:
                currentOffsetY -= 10;
                break;
            case LEFT_DIR:
                currentOffsetX -= 10;
                break;
            case RIGHT_DIR:
                currentOffsetX += 10;
                break;
        }*/
    }
    switch (dirs[0]) {
        case DOWN_DIR:
            currentOffsetY += 10;
            break;
        case UP_DIR:
            currentOffsetY -= 10;
            break;
        case LEFT_DIR:
            currentOffsetX -= 10;
            break;
        case RIGHT_DIR:
            currentOffsetX += 10;
            break;
    }
    if(currentOffsetX == appleLocation[0] && currentOffsetY == appleLocation[1]) {
        drawApple();
        snakeAteApple = true;
    }
    if(
            std::find(snakeX.begin(), snakeX.end(), currentOffsetX) != snakeX.end() && // MUST be on the same index!!!
            std::find(snakeY.begin(), snakeY.end(), currentOffsetY) != snakeY.end()    // todo fix that crap
            ) {
                drawText(10, 10, "you fucked up", ST7735_WHITE, ST7735_BLACK, 1);
                return false;
    }
    drawSnake();
    return true;
}


void gpio_callback(uint gpio, uint32_t events) {
    if ((to_ms_since_boot(get_absolute_time())-time)>delayTime) {
        time = to_ms_since_boot(get_absolute_time());
        switch (gpio) {
            case 1: // down
                if(dirs.back() != UP_DIR) dirs.push_back(DOWN_DIR);
                //currentOffset = 0;
                break;
            case 22: // right
                if(dirs.back() != LEFT_DIR) dirs.push_back(RIGHT_DIR);
                //currentOffset = 0;
                break;
            case 21: // up
                if(dirs.back() != DOWN_DIR) dirs.push_back(UP_DIR);
                //currentOffset = 10;
                break;
            case 20: // left
                if(dirs.back() != RIGHT_DIR) dirs.push_back(LEFT_DIR);
                //currentOffset = 0;
                break;
            default:
                return;
        }
    }
}


void seed_random_from_rosc()
{
    uint32_t random = 0x811c9dc5;
    uint8_t next_byte = 0;
    volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < 8; k++) {
            next_byte = (next_byte << 1) | (*rnd_reg & 1);
        }

        random ^= next_byte;
        random *= 0x01000193;
    }

    srand(random);
}

void drawApple() {
    appleLocation[0] = (trunc(((double) (rand() % 121)) / 10)) * 10;
    appleLocation[1] = (trunc(((double) (rand() % 151)) / 10)) * 10;
    fillRect(appleLocation[0] + 4, // Move right
             appleLocation[1] + 5,
             10,
             10,
             ST7735_RED
    );
}

int main() {
    stdio_init_all();
    seed_random_from_rosc();
    // SPI init ( for the screen)
    spi_init(spi0, 15 * 1000000);                // SPI with 133 max
    gpio_set_function(16, GPIO_FUNC_SPI);
    gpio_set_function(19,GPIO_FUNC_SPI);
    gpio_set_function(18, GPIO_FUNC_SPI);
    tft_spi_init();

    // buttons init
    int buttons_gpios[] = {1, 22, 21, 20};

    for (int gpio : buttons_gpios) {
        gpio_pull_up(gpio);
        gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    }

    //screen init
    TFT_BlackTab_Initialize();
    setTextWrap(true);
    sleep_ms(100);
    fillScreen(ST7735_BLACK);

    drawRectWH(3, 4, tft_width - 6, tft_height - 8, ST7735_WHITE);

    drawApple();
    dirs.push_back(RIGHT_DIR);
    snakeX.push_back(0);
    snakeY.push_back(0);

    struct repeating_timer timer;
    add_repeating_timer_ms(snakeRefreshRate, repeating_timer_callback, NULL, &timer);
    while(true) {}
}