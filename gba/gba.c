#include "myLib.h"

unsigned short *videoBuffer = (unsigned short*) 0x6000000;

void setPixel(int row, int col, unsigned short color)
{
    videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawScreen(const unsigned short *image)
{
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = DMA_ON | 38400;
}

void drawRectangle(int x, int y, int width, int height, vuint16_t color)
{
    for( int j = 0; j < height; j++ )
    {
        DMA[3].src = &color;
        DMA[3].dst = videoBuffer + (y + j) * 240 + x;
        DMA[3].cnt = DMA_ON | width | DMA_SOURCE_FIXED;
    }
}

void drawImage3(int x, int y, int width, int height, const unsigned short *image)
{
    for( int j = ((y >= UBound) ? (0) : (UBound - y)); j < height; j++ )
    {
        if( x < 240 && x >= 0 )
        {
            DMA[3].src = image + j * width;
            DMA[3].dst = videoBuffer + ((y + j) * 240) + x;
            if( 240 - x < width )
            {
                DMA[3].cnt = DMA_ON | (240 - x);
            }
            else
            {
                DMA[3].cnt = DMA_ON | width;
            }
        }
        else if( x < 0 && (width + x) > 0 )
        {
            DMA[3].src = image + j * width - x;
            DMA[3].dst = videoBuffer + ((y + j) * 240);
            DMA[3].cnt = DMA_ON | (width + x);
        }
    }
}

void waitForVblank(void)
{
    while( SCANLINECOUNTER > 160 )
        ;
    while( SCANLINECOUNTER < 160 )
        ;
}

