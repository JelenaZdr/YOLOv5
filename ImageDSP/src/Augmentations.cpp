#include <cstdlib>
#include <time.h>
#include "Augmentations.h"
#include"DCT.h"

using namespace std;

void myAugmentation0(  //dct lowpass compression
	uchar in_out_y[],
	int x_size,
	int y_size
) {
    performDCTandIDCT(in_out_y, x_size, y_size, 5);
    //qCritical()<<"PRINT";
}


void myAugmentation1( //exposure
    uchar in_out_y[],
	int x_size,
	int y_size,
    uchar threshold // [0, 255]
)
{
    int pr;
    for(int i=0; i<x_size*y_size; i++){
        pr=in_out_y[i]+threshold-150;
        if(pr>255)
            pr=255;
        if(pr<0)
            pr=0;
        in_out_y[i]=pr;
    }
}

void myAugmentation2(   //cutout
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    int cutout_size

)
{
    memcpy(output, input, x_size * y_size * 3);

    int rand_num_x, rand_num_y;
    srand(time(0));
    for(int i=0; i<6; i++){
    rand_num_y = rand() % (y_size-cutout_size);
    rand_num_x = rand() % (x_size-cutout_size);

    int cutoutX = rand_num_x;
    int cutoutY = rand_num_y;

    for(int y = cutoutY; y < cutoutY + cutout_size; y++){
        for(int x = cutoutX; x < cutoutX + cutout_size; x++){
            output[(y * x_size + x) * 3 + 0] = 0;
            output[(y * x_size + x) * 3 + 1] = 0;
            output[(y * x_size + x) * 3 + 2] = 0;
        }
    }}
}

void myAugmentation3(  //cut+aspect
        const uchar input[],
        int x_size,
        int y_size,
        uchar output[]
        )
{
    const int cropX = 0;   // x-coordinate of the top-left corner of the crop
    const int cropY = 0;   // y-coordinate of the top-left corner of the crop
    const int crop=min(x_size, y_size);
    for (int y = 0; y < 640;y++) {
           for (int x = 0; x < 640; x++) {
               const int srcIndex = ((y + cropY) * x_size + (x + cropX)) * 3;
               const int dstIndex = (y * crop + x) * 3;
               memcpy(&output[dstIndex], &input[srcIndex], 3);
           }
    }
}

