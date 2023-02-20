#include <math.h>

#include "YuvLibFile.h"
#include "YuvLibRgb.h"

#define FRAME_WIDTH  1280
#define FRAME_HEIGHT  720
#define PIXEL_SIZE      3
#define BLOCK_SIZE    120

#define CIRCLE_MARGIN 30
#define LINE_THICK     5

/**
 这种写法其实并不严格。
 作为测试还是可以的。
 */
static int YUV_RgbRevertPixel(char* pRgb)
{
    for (int i=0; i<LINE_THICK; i++)
    {
        pRgb[0] = 0x80;
        pRgb[1] = 0x80;
        pRgb[2] = 0x80;
        
        pRgb += 3;
    }
}


static int YUV_RgbCircleY(char* pRgb, int start, int end)
{
    if (start > end)
    {
        int temp = start;
        start = end;
        end = temp;
    }
    
    for (int i=start; i<=end; i++)
    {
        YUV_RgbRevertPixel(pRgb+i*PIXEL_SIZE);
    }
    
}

static int YUV_RgbAdd1Circle(char* pCenter,
                            int radius, int pitch)
{
    int prex = radius;
    int x3 = radius;
    
    /* 0 line */
    YUV_RgbCircleY(pCenter,  prex,  x3);
    YUV_RgbCircleY(pCenter, -prex, -x3);
    
    for (int j=1; j<=radius; j++)
    {
        prex = x3;
        x3   = (int)(sqrt(radius*radius-j*j)+0.5F);
        YUV_RgbCircleY(pCenter+( j)*pitch,  prex,  x3);
        YUV_RgbCircleY(pCenter+( j)*pitch, -prex, -x3);
        YUV_RgbCircleY(pCenter+(-j)*pitch,  prex,  x3);
        YUV_RgbCircleY(pCenter+(-j)*pitch, -prex, -x3);
    }

    return 0;
}

static int YUV_RgbAdd5Circles(char* pRgb,
                            int width,
                            int height)
{
    int pitch=width*3;
    int radius;
    char* pCenter;
    
    radius = (height-CIRCLE_MARGIN*2)/2;
    pCenter = pRgb+pitch*(height/2)+pitch/2;
    YUV_RgbAdd1Circle(pCenter, radius, pitch);
    
    return 0;
}


int main(int argc, char** argv)
{
    char caRgbFile[FILE_NAME_SIZE] = {0};
    int nRgbSize = FRAME_WIDTH*FRAME_HEIGHT*PIXEL_SIZE;
    char* pRgb = (char*)malloc(nRgbSize);
    
    YUV_Rgb_CreateBuffer(pRgb, FRAME_WIDTH, FRAME_HEIGHT, PIXEL_SIZE, BLOCK_SIZE);
    YUV_RgbAdd5Circles(pRgb, FRAME_WIDTH, FRAME_HEIGHT);
    
    YUV_CreateFileNameFrame(caRgbFile, "Rgb", FRAME_WIDTH, FRAME_HEIGHT);
    YUV_WriteToFile(caRgbFile, pRgb, nRgbSize);
    
    return 0;
}


