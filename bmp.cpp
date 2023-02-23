//
// Created by David on 19.01.2023.
//
#include "bmp.h"
#include "microbuffer.h"

BitmapFile::BitmapFile(const char *file_name) {
    int file = pico_open(file_name, LFS_O_RDWR | LFS_O_CREAT);
    int file_size = pico_size(file);
    buffer = (uint8_t*) malloc(file_size * sizeof(uint8_t));

    uint16_t fileHeader;
    int      bmpWidth, bmpHeight;   // W+H in pixels
    uint16_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbuffer[3 * pico_size(file)]; // pixel buffer (R+G+B per pixel)
    uint32_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    bool     flip    = true;        // BMP is stored bottom-to-top
    int      w, h, row, col;
    uint8_t  r, g, b;
    uint32_t pos = 0;

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
                width = bmpWidth;
                height = bmpHeight;

                int x = 0;
                int y = 0;

                pico_lseek(file, 20, LFS_SEEK_CUR); // seek 8 bytes ( skip file size and author info )

                pico_read(file, buffer, 3 * file_size);
            }
        } else {
            printf("unsupported bmp - use a 24bit uncompressed bmp");
        }
    } else {
        printf("Wrong file header - not a BMP");
    }

};

uint16_t Color565(int16_t r, int16_t g, int16_t b){
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void BitmapFile::Draw(const std::shared_ptr<Framebuffer>& fb, int x, int y, Rotation rot) {
    int buffidx = 0; // Set index to beginning

    if(rot == Rotation::deg180) {
        for (int row=0; row < height; row++) { // For each scanline...

            for (int col = width; col > 0; col--) { // For each pixel...
                int b = buffer[buffidx++];
                int g = buffer[buffidx++];
                int r = buffer[buffidx++];
                fb->draw_pixel(col, row, Color(r, b, g));
            } // end pixel
            buffidx += 2;
        } // end scanline
        return;
    }

    for (int row = height; row > 0; row--) { // For each scanline...

        for (int col=0; col<width; col++) { // For each pixel...
            int b = buffer[buffidx++];
            int g = buffer[buffidx++];
            int r = buffer[buffidx++];
            fb->draw_pixel(col, row, Color(r, b, g));
        } // end pixel
        buffidx += 2;
    } // end scanline

}