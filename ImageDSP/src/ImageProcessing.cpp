
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "Augmentations.h"
#include <cstdlib>
#include"algorithm"
#include <QDebug>

void imageProcessingFun(
	const QString& prog_name,
	QImage& out_img,
	const QImage& in_img,
	const QVector<double>& params
) {
	int x_size = in_img.width();
	int y_size = in_img.height();

    if(prog_name == "DCT"){
		// Create empty output image.
		out_img = QImage(x_size, y_size, in_img.format());
		
		// Create buffers for YUV image.
		uchar* Y_buff = new uchar[x_size*in_img.height()];
		char* U_buff = new char[x_size*in_img.height()/4];
		char* V_buff = new char[x_size*in_img.height()/4];

		// Convert input image to YUV420 image.
		RGBtoYUV420(in_img.bits(), x_size, y_size, Y_buff, U_buff, V_buff);

		myAugmentation0(Y_buff, x_size, y_size);
		
		// Zero out U and V component.
		procesing_YUV420(Y_buff, U_buff, V_buff, x_size, y_size, 1, 1, 1);

		// Convert YUV image back to RGB.
		YUV420toRGB(Y_buff, U_buff, V_buff, x_size, y_size, out_img.bits());

		// Delete used memory buffers.
		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;
    }else if(prog_name == "Exposure"){

        out_img = QImage(x_size, y_size, in_img.format());
		uchar threshold = params[0];
        uchar* Y_buff = new uchar[x_size*in_img.height()];
        char* U_buff = new char[x_size*in_img.height()/4];
        char* V_buff = new char[x_size*in_img.height()/4];

        // Convert input image to YUV420 image.
        RGBtoYUV420(in_img.bits(), x_size, y_size, Y_buff, U_buff, V_buff);
        myAugmentation1(Y_buff, x_size, y_size, threshold);
        procesing_YUV420(Y_buff, U_buff, V_buff, x_size, y_size, 1, 1, 1);

        // Convert YUV image back to RGB.
        YUV420toRGB(Y_buff, U_buff, V_buff, x_size, y_size, out_img.bits());



	}
    else if(prog_name=="Cutout"){
        out_img = QImage(x_size, y_size, in_img.format());

        myAugmentation2(in_img.bits(), x_size, y_size, out_img.bits(), 70);

    }
    else if(prog_name=="Cut+Aspect"){
        int size_p;
        if(x_size<y_size)
            size_p=x_size;
        else
            size_p=y_size;
        out_img=QImage(size_p, size_p, in_img.format());
        myAugmentation3(in_img.bits(), x_size, y_size, out_img.bits());
    }


}
