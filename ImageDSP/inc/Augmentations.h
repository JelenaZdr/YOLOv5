
#pragma once

#include <QDebug>

void myAugmentation0(
	uchar in_out_y[],
	int x_size,
	int y_size
);

void myAugmentation1(
	uchar in_out_rgb[],
	int x_size,
	int y_size,
    uchar threshold // [0, 255]
);

void myAugmentation2(

        const uchar input[],
        int x_size,
        int y_size,
        uchar output[],
        int cutout_size

        );
void myAugmentation3(
        const uchar input[],
        int x_size,
        int y_size,
        uchar output[]
        );
