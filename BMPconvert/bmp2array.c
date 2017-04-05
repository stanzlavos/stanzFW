#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BUFFPIXEL       20

#define COLOR_ARRAY     0
#define COLOR_FILE      1

uint16_t read16(FILE *fp) 
{
    uint16_t result;
    fread((&((uint8_t *)&result)[0]), 1, 1, fp);
    fread((&((uint8_t *)&result)[1]), 1, 1, fp);
    
    return result;
}

uint32_t read32(FILE *fp) {

    uint32_t result;
    fread((&((uint8_t *)&result)[0]), 1, 1, fp);
    fread((&((uint8_t *)&result)[1]), 1, 1, fp);
    fread((&((uint8_t *)&result)[2]), 1, 1, fp);
    fread((&((uint8_t *)&result)[3]), 1, 1, fp);

    return result;
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) 
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

FILE *colorFile = NULL;

void printCOLOR(uint16_t val)
{
    if(!colorFile)
    {
        static uint8_t col_cnt = 0;

        printf("0x%04X, ", val);

        col_cnt++;
        if((col_cnt%32) == 0)
            printf("\n");
    }
    else
    {
        if(fwrite(&val, 2, 1, colorFile) != 1)
            printf("\n*** Palette file write issue !!! ***\n"); 
    }
}

void readBMPdumpCOLOR(uint8_t type, char *filename) 
{
    FILE     *bmpFile = NULL;
    int      bmpWidth, bmpHeight;   // W+H in pixels
    uint8_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
    uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    bool     goodBmp = false;       // Set to true on valid header parse
    bool     flip    = true;        // BMP is stored bottom-to-top
    int      w, h, row, col;
    uint8_t  r, g, b;
    uint32_t pos = 0;

    if ((bmpFile = fopen(filename, "r")) == NULL) {
        printf("*** File not found!!! ***\n");
        return;
    }

    if(type == COLOR_FILE)
    {
        char *ptr = strchr(filename, '.');
        ptr[1] = '\0';
        strcat(filename, "palette");

        if ((colorFile = fopen(filename, "w+")) == NULL) {
            printf("*** Palette file open failed !!! ***\n");
            return;
        } 
    }

    // Parse BMP header
    if(read16(bmpFile) == 0x4D42) { // BMP signature
        (void)read32(bmpFile);              // Read & ignore file size
        (void)read32(bmpFile);              // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile);   // Start of image data
        // Read DIB header
        (void)read32(bmpFile);              // Read & ignore header size
        bmpWidth  = read32(bmpFile);
        bmpHeight = read32(bmpFile);
        if(read16(bmpFile) == 1) { // # planes -- must be '1'
            bmpDepth = read16(bmpFile); // bits per pixel
            //printf("\nBit Depth: %d\n", bmpDepth);
            if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

                goodBmp = true; // Supported BMP format -- proceed!
                //printf("\nImage size: %dx%d", bmpWidth, bmpHeight);

                printCOLOR(bmpWidth);
                printCOLOR(bmpHeight);

                // BMP rows are padded (if needed) to 4-byte boundary
                rowSize = (bmpWidth * 3 + 3) & ~3;

                // If bmpHeight is negative, image is in top-down order.
                // This is not canon but has been observed in the wild.
                if(bmpHeight < 0) {
                    bmpHeight = -bmpHeight;
                    flip      = false;
                }

                // Crop area to be loaded
                w = bmpWidth;
                h = bmpHeight;
                //if((x+w-1) >= tft.width())  w = tft.width()  - x;
                //if((y+h-1) >= tft.height()) h = tft.height() - y;

                for (row=0; row<h; row++) { // For each scanline...

                    // Seek to start of scan line.  It might seem labor-
                    // intensive to be doing this on every line, but this
                    // method covers a lot of gritty details like cropping
                    // and scanline padding.  Also, the seek only takes
                    // place if the file position actually needs to change
                    // (avoids a lot of cluster math in SD library).
                    if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else     // Bitmap is stored top-to-bottom
                        pos = bmpImageoffset + row * rowSize;
                    
                    if(ftell(bmpFile) != pos) { // Need seek?
                        fseek(bmpFile, pos, SEEK_SET);
                        buffidx = sizeof(sdbuffer); // Force buffer reload
                    }

                    for (col=0; col<w; col++) { // For each pixel...
                        // Time to read more pixel data?
                        if (buffidx >= sizeof(sdbuffer)) { // Indeed
                            fread(sdbuffer, 1, sizeof(sdbuffer), bmpFile);
                            buffidx = 0; // Set index to beginning
                        }

                        // Convert pixel from BMP to TFT format, push to display
                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];
                        printCOLOR(color565(r,g,b));
                    } // end pixel
                } // end scanline
            } // end goodBmp
        }
    }

    //bmpFile.close();
    if(bmpFile)
        fclose(bmpFile);
    if(colorFile)
        fclose(colorFile);
    if(!goodBmp) printf("\n*** BMP format not recognized!!! ***\n");
}

void readBMPdumpBMP(char *filename)
{
    uint8_t     col_cnt     = 0;
    FILE        *fp         = fopen(filename, "r");

    if(fp == NULL)
    {
        printf("\n*** File open failed !!! ***\n");
        return;
    }

    while(1)
    {
        uint8_t byte_read = 0;
        if(!fread((void *)&byte_read, 1, 1, fp))
            break;


        printf("0x%02X, ", byte_read);

        col_cnt++;
        if((col_cnt%32) == 0)
            printf("\n");
    }
}


int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("\n");
        printf("Usage : ./bmp2array <opt num> <bmp file name>\n");
        printf("        <opt num>           <action>\n");
        printf("            1               generate bmp array\n");
        printf("            2               generate color array\n");
        printf("            3               generate color file\n\n");
        
        return -1;
    }

    printf("\n\n{\n");

    uint8_t opt = atoi(argv[1]);
    if(opt == 1)
    {
        readBMPdumpBMP(argv[2]);
    }
    else if(opt == 2)
    {
        readBMPdumpCOLOR(COLOR_ARRAY, argv[2]);
    }
    else if(opt == 3)
    {
        readBMPdumpCOLOR(COLOR_FILE, argv[2]);
    }

    printf("\n}\n\n");

    return 0;
}
