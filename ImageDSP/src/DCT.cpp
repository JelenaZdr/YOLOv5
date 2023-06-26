#include"DCT.h"
#include<cmath>


#define PI 3.14159265359



static void doZigZag(short block[], int N, int B)
{
    /* TO DO */
    int currNum = 0;
    int currDiagonalWidth = 1;
    int i;
    int row;
    int col;

    while (currDiagonalWidth<N)
    {
        for (i = 0; i<currDiagonalWidth; i++)
        {
            if (currDiagonalWidth % 2 == 1)
            {
                row = currDiagonalWidth - i - 1;
                col = i;
            }
            else
            {
                row = i;
                col = currDiagonalWidth - i - 1;
            }
            // OBRADA PIKSELA SA KOORDINATAMA row I col
            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }
        currDiagonalWidth++;
    }
    while (currDiagonalWidth> 0)
    {
        for (i = currDiagonalWidth; i> 0; i--)
        {
            if (currDiagonalWidth % 2 == 1){
                row = N - currDiagonalWidth + i - 1;
                col = N - i;
            }
            else
            {
                row = N - i;
                col = N - currDiagonalWidth + i - 1;
            }
            // OBRADA PIKSELA SA KOORDINATAMA row I col
            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }

        currDiagonalWidth--;
    }

}



void performDCTandIDCT(uchar* input, int xSize, int ySize, int N)
{
    /* Create NxN buffer for one input block */
    uchar* inBlock = new uchar[N*N];

    /* Generate DCT kernel */
    double* DCTKernel = new double[N*N];
    GenerateDCTmatrix(DCTKernel, N);

    /* Create buffer for DCT coefficients */
    short* dctCoeffs = new short[N*N];

    /* Extend image borders */
    int xSize2, ySize2;
    uchar* input2;
    extendBorders(input, xSize , ySize, N, &input2, &xSize2, &ySize2);

    for (int y = 0; y < ySize2/N; y++)
    {
        for (int x = 0; x < xSize2/N; x++)
        {

            /* Fill input block buffer */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    inBlock[j*N+i] = input2[(N*y+j)*(xSize2)+N*x+i];
                }
            }

            /* Invoke DCT */
            DCT(inBlock, dctCoeffs, N, DCTKernel);


            doZigZag(dctCoeffs, N, 3);

            /* Invoke IDCT */
            IDCT(dctCoeffs, inBlock, N, DCTKernel);

            /* Write output values to output image */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    input2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
                }
            }
        }
    }

    cropImage(input2, xSize2, ySize2, input, xSize, ySize);

    /* Delete used memory buffers coefficients */
    delete[] dctCoeffs;
    delete[] inBlock;
    delete[] DCTKernel;
    delete[] input2;
}



void GenerateDCTmatrix(double* DCTKernel, int order)
{
    int i, j;
    int N;
    N = order;
    double alpha;
    double denominator;
    for (j = 0; j <= N - 1; j++)
    {
        DCTKernel[0, j] = sqrt(1 / (double)N);
    }
    alpha = sqrt(2 / (double)N);
    denominator = (double)2 * N;
    for (j = 0; j <= N - 1; j++)
        for (i = 1; i <= N - 1; i++)
        {
            DCTKernel[i*N + j] = alpha * cos(((2 * j + 1) * i * PI) / denominator);
        }
}

void DCT(const uchar input[], short output[], int N, double* DCTKernel)
{
    double* temp = new double[N*N];
    double* DCTCoefficients = new double[N*N];

    double sum;
    for (int i = 0; i <= N - 1; i++)
    {
        for (int j = 0; j <= N - 1; j++)
        {
            sum = 0;
            for (int k = 0; k <= N - 1; k++)
            {
                sum = sum + DCTKernel[i*N+k] * (input[k*N+j]-128.0);
            }
            temp[i*N + j] = sum;
        }
    }

    for (int i = 0; i <= N - 1; i++)
    {
        for (int j = 0; j <= N - 1; j++)
        {
            sum = 0;
            for (int k = 0; k <= N - 1; k++)
            {
                sum = sum + temp[i*N+k] * DCTKernel[j*N+k];
            }
            DCTCoefficients[i*N+j] = sum;
        }
    }

    for(int i = 0; i < N*N; i++)
    {
        output[i] = floor(DCTCoefficients[i]+0.5);
    }

    delete[] temp;
    delete[] DCTCoefficients;

    return;
}

void IDCT(const short input[], uchar output[], int N, double* DCTKernel)
{
    /* TO DO */
    double* temp = new double[N*N];
    double* DCTCoefficients = new double[N*N];

    double sum;
    for (int i = 0; i <= N - 1; i++)
    {
        for (int j = 0; j <= N - 1; j++)
        {
            sum = 0;
            for (int k = 0; k <= N - 1; k++)
            {
                sum = sum + DCTKernel[k*N+i] * (input[k*N+j]);
            }
            temp[i*N + j] = sum;
        }
    }

    for (int i = 0; i <= N - 1; i++)
    {
        for (int j = 0; j <= N - 1; j++)
        {
            sum = 0;
            for (int k = 0; k <= N - 1; k++)
            {
                sum = sum + temp[i*N+k] * DCTKernel[k*N+j];
            }
            DCTCoefficients[i*N+j] = sum;
        }
    }

    for(int i = 0; i < N*N; i++)
    {
        output[i] = floor(DCTCoefficients[i]+0.5)+128;
    }

    delete[] temp;
    delete[] DCTCoefficients;

    return;
}


void extendBorders(uchar* input, int xSize, int ySize, int N, uchar** p_output, int* newXSize, int* newYSize)
{
    int deltaX = N-xSize%N;
    int deltaY = N-ySize%N;

    *newXSize = xSize+deltaX;
    *newYSize = ySize+deltaY;

    uchar* output = new uchar[(xSize+deltaX)*(ySize+deltaY)];

    for (int i=0; i<ySize; i++)
    {
        memcpy(&output[i*(xSize+deltaX)], &input[i*(xSize)], xSize);
        if(deltaX != 0)
        {
            memset(&output[i*(xSize+deltaX)+xSize], output[i*(xSize+deltaX)+xSize - 1], deltaX);
        }
    }

    for (int i=0; i<deltaY; i++)
    {
        memcpy(&output[(i + ySize)*(xSize+deltaX)], &output[(ySize)*(xSize+deltaX)], xSize+deltaX);
    }

    *p_output = output;
}

void cropImage(uchar* input, int xSize, int ySize, uchar* output, int newXSize, int newYSize)
{
    for (int i=0; i<newYSize; i++)
    {
        memcpy(&output[i*(newXSize)], &input[i*(xSize)], newXSize);
    }
}

