#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hw.h"
#include "ST7735_TFT.h"
#include "pico_hal.h"

#define BUFFPIXEL 20480


int16_t Color565(int16_t r, int16_t g, int16_t b){
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(spi0, 15 * 1000000);                // SPI with 133 max
    gpio_set_function(16, GPIO_FUNC_SPI);
    gpio_set_function(19,GPIO_FUNC_SPI);
    gpio_set_function(18, GPIO_FUNC_SPI);
    tft_spi_init();

    //TFT_RedTab_Initialize();
    //TFT_ST7735B_Initialize();
    TFT_BlackTab_Initialize();

    setTextWrap(true);
    sleep_ms(100);
    fillScreen(ST7735_BLACK);
    int count = 0;

    while(1) {
        if (pico_mount(false) != LFS_ERR_OK) {
            printf("Error mounting FS\n");
            exit(-1);
        }
        int file = pico_open("arrow", LFS_O_RDWR | LFS_O_CREAT);
        uint16_t fileHeader;
        int      bmpWidth, bmpHeight;   // W+H in pixels
        uint8_t  bmpDepth;              // Bit depth (currently must be 24)
        uint32_t bmpImageoffset;        // Start of image data in file
        uint32_t rowSize;               // Not always = bmpWidth; may have padding
        uint8_t  sdbuffer[3*pico_size(file)]; // pixel buffer (R+G+B per pixel)
        uint32_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
        //bool     goodBmp = false;       // Set to true on valid header parse
        bool     flip    = true;        // BMP is stored bottom-to-top
        int      w, h, row, col;
        uint8_t  r, g, b;
        uint32_t pos = 0;

        uint32_t uselessInt = 0;

        pico_read(file, &fileHeader, 2);
        if(fileHeader == 0x4D42) {
            pico_lseek(file, 8, LFS_SEEK_CUR); // seek 8 bytes ( skip file size and author info )
            pico_read(file, &bmpImageoffset, 4);
            pico_lseek(file, 4, LFS_SEEK_CUR); // seek 8 bytes ( skip header size )
            pico_read(file, &bmpWidth, 4);
            pico_read(file, &bmpHeight, 4);
            uint16_t planes;
            pico_read(file, &planes, 2);
            if(planes == 1) {
                pico_read(file, &bmpDepth, 2);
                uint32_t compressed;
                pico_read(file, &compressed, 4);
                if(bmpDepth == 24 && compressed == 0) { // if bit depth is 24 and uncompressed
                    rowSize = (bmpWidth * 3 + 3) & ~3;
                    printf("%lu", rowSize);
                    // If bmpHeight is negative, image is in top-down order.
                    // This is not common but has been observed in the wild.
                    if(bmpHeight < 0) {
                        bmpHeight = -bmpHeight;
                        flip      = false;
                    }
                    w = bmpWidth;
                    h = bmpHeight;

                    int x = 0;
                    int y = 0;

                    if((x+w-1) >= tft_width) w = tft_width - x;
                    if((y+h-1) >= tft_height) h = tft_height - y;

                    ///*tft.startWrite();
                    setAddrWindow(x, y, w - 1, h - 1);
                    pico_lseek(file, 20, LFS_SEEK_CUR); // seek 8 bytes ( skip file size and author info )

                    pico_read(file, &sdbuffer, sizeof(sdbuffer));
                    buffidx = 0; // Set index to beginning

                    for (row=0; row<h; row++) { // For each scanline...

                        // Seek to start of scan line.  It might seem labor-
                        // intensive to be doing this on every line, but this
                        // method covers a lot of gritty details like cropping
                        // and scanline padding.  Also, the seek only takes
                        // place if the file position actually needs to change
                        // (avoids a lot of cluster math in SD library).
                        /*if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
                            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                        else     // Bitmap is stored top-to-bottom
                            pos = bmpImageoffset + row * rowSize;
                        if(pico_tell(file) != pos) { // Need seek?
                            //tft.endWrite();
                            pico_lseek(file, pos, LFS_SEEK_SET); // TODO - double check whence
                            buffidx = sizeof(sdbuffer); // Force buffer reload
                        }*/

                        for (col=0; col<w; col++) { // For each pixel...
                            // Time to read more pixel data?
                            /*if (buffidx >= sizeof(sdbuffer)) { // Indeed
                                pico_read(file, &sdbuffer, sizeof(sdbuffer));
                                buffidx = 0; // Set index to beginning
                                //tft.startWrite();
                            }*/

                            // Convert pixel from BMP to TFT format, push to display
                            b = sdbuffer[buffidx++];
                            g = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            //printf("push color r - %d g - %d b - %d", r, g ,b);
                            pushColor(Color565(r, g, b));
                            //tft.pushColor(tft.color565(r,g,b));
                        } // end pixel
                        buffidx += 2;
                    } // end scanline
                    /*tft.endWrite(); TODO - ofc add the tft lib staff also some frametime crap
                    Serial.print(F("Loaded in "));
                    Serial.print(millis() - startTime);
                    Serial.println(" ms");*/

                }
            } else {
                printf("unsupported bmp - use a 24bit uncompressed bmp");
            }
        } else {
            printf("Wrong file header - not a BMP");
        }

        //setRotation(i);
        //fillScreen(ST7735_BLACK);
        /*switch (count % 6) {
            case 0:
                fillScreen(ST7735_BLUE);
                break;
            case 1:
                fillScreen(ST7735_RED);
                break;
            case 2:
                fillScreen(ST7735_GREEN);
                break;
            case 3:
                fillScreen(ST7735_MAGENTA);
                break;
            case 4:
                fillScreen(ST7735_YELLOW);
                break;
            case 5:
                fillScreen(ST7735_WHITE);
                break;
        }*/
        /*drawText(10, 10, "Test over!", ST7735_WHITE, ST7735_BLACK, 1);
        drawFastHLine(0, 0, 80, ST7735_CYAN);
        drawFastHLine(0, 25, 80, ST7735_CYAN);
        drawFastVLine(0, 0, 25, ST7735_CYAN);
        drawFastVLine(80, 0, 25, ST7735_CYAN);*/




        /*int x = 0;
        int y = 0;
        int w = 20;
        int h = 20;
        int maxX = tft_width - w;
        int maxY = tft_height - h;

        fillRect(x, y, w, h, ST7735_BLUE);

        for (; x < maxX; ++x) {
            drawFastVLine(x - 1, y, h, ST7735_BLACK);
            drawFastVLine(x + w, y, h, ST7735_BLUE);
            sleep_ms(5);
        }
        drawFastVLine(x - 1, y, h, ST7735_BLACK);
        for (; y < maxY; ++y) {
            drawFastHLine(x, y - 1, h, ST7735_BLACK);
            drawFastHLine(x, y + h, h, ST7735_BLUE);
            sleep_ms(5);
        }
        drawFastHLine(x, y - 1, h, ST7735_BLACK);
        for (; x > 0; --x) {
            drawFastVLine(x + w, y, h, ST7735_BLACK);
            drawFastVLine(x - 1, y, h, ST7735_BLUE);
            sleep_ms(5);
        }
        drawFastVLine(x, y + h + 1, h, ST7735_BLACK);
        for (; y > 0; --y) {
            drawFastHLine(x, y + h, h, ST7735_BLACK);
            drawFastHLine(x, y - 1, h, ST7735_BLUE);
            sleep_ms(5);
        }*/
        /*for (w = 0; w < tft_width; ++w) {
            fillRect(w - 2, h, 2, 8, ST7735_BLACK);
            drawText(w, h, "Hello world", ST7735_WHITE, ST7735_BLACK, 1);
        }
        for (h = 0; h < tft_height; ++h) {
            //fillRect(0, h + 8, 70, 2, ST7735_BLACK);
            //drawText(w, h, "Hello world", ST7735_WHITE, ST7735_BLACK, 1);
            fillRect(20, 20, 50, 50, ST7735_BLACK);
            fillRect(20, 20, 50, 50, ST7735_GREEN);
            asm volatile("nop");
        }*/
        //sleep_ms(1000);
        count ++;
        //fillScreen(ST7735_BLACK);
    }
    //return 0;
}
