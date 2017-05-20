#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//================================ IMAGE FUNCTIONS ================================
void imshow(IplImage* image) {
	cvNamedWindow("Display window", CV_WINDOW_AUTOSIZE);
	cvShowImage("Display window", image);
	cvWaitKey(0);
}

int getPixelValue(IplImage* im, int x, int y) {
	CvScalar pixelObject = cvGet2D(im, x, y);
	return (int) pixelObject.val[0];
}

IplImage* imread(char* pathToImage) {
	return cvLoadImage(pathToImage, CV_LOAD_IMAGE_GRAYSCALE);
}

int getRows(IplImage* image) {
	CvSize dim = cvGetSize(image);
	return dim.height;
}

int getCols(IplImage* image) {
	CvSize dim = cvGetSize(image);
	return dim.width;
}

//================================ NUMBER FUNCTIONS ================================
int decimalToBinary(int n) {
    int remainder; 
 	int binary = 0, i = 1;
  
    while(n != 0) {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
    }
    return binary;
}

int binaryToDecimal(char *bin) {
	int ASCII_ZERO = 48;
	int dec = 0;
	int j;
	int p = 0;
	int i = strlen(bin);
	for (j = i-1; j >= 0; j-- ) {
	 dec +=  (bin[j] - ASCII_ZERO) * (int)pow((double)2, p);
	 //Increment the position variable
	 p++;
 	}
 	return dec;
}

void inttostr(int integer, char string[8]) {
	sprintf(string, "%d", integer);
}

void add(char* bin1, char* bin2, char output[8]) {
	int dec1 = binaryToDecimal(bin1);
	int dec2 = binaryToDecimal(bin2);
	int sumDec = dec1 + dec2;
	int sumBin = decimalToBinary(sumDec);
	inttostr(sumBin, output);
}

char* LSB(char* pixelValue) {
	return &pixelValue[strlen(pixelValue) - 1];
}

char f(char* bin1, char* bin2) {
	int dec1 = binaryToDecimal(bin1);
	int dec2 = binaryToDecimal(bin2);
	double n1 = floor(dec1 / 2.0) + dec2;
	int output = decimalToBinary((int) n1);
	char str[8];
	snprintf(str, 8, "%d", output);
	char* lsb = LSB(str);
	char c = lsb[0];
	return c;
}

int main(int argc, char** argv) {
	IplImage* image;
	int nrow, ncol;

	image = imread("im.jpg");
	nrow = getRows(image);
	ncol = getCols(image);

	char MESSAGE [] = "01101001001000000110000101101101001000000111010001101000011001010010000001100111011100100110010101100001011100";
	int MESSAGE_LENGTH = strlen(MESSAGE);
	int count = 0, flag = 0;

	int i, j;
	for (i = 0; i < nrow - 1; i++) {
			
		if (flag == 1) {
			break;
		}

		for (j = 0; j < ncol; j++) {

			if (flag == 1) {
				break;
			}

			char bin1[8];
			char bin2[8];
			inttostr(decimalToBinary(getPixelValue(image, i, j)), bin1);
			inttostr(decimalToBinary(getPixelValue(image, i + 1, j)), bin2);

			char m1 = MESSAGE[count];
			char m2 = MESSAGE[count + 1];

			char* lsb = LSB(bin1);

			char* y1;
			char* y2;

			char temp[8];
			if (m1 == *lsb) {
				if (m2 != f(bin1, bin2)) {
					add(bin1, "1", temp);
					y2 = temp;
				} else {
					y2 = bin2;
				}
				y1 = bin2;
			} else {
				add(bin1, "-1", temp);
				if (m2 == f(temp, bin2)) {
					add(bin1, "-1", temp);
					y1 = temp;
				}
				else {
					add(bin1, "1", temp);
					y1 = temp;
				}
				y2 = bin2;
			}

			int newPixelValue1 = binaryToDecimal(y1);
			int newPixelValue2 = binaryToDecimal(y2);

			CvScalar scalar1 = cvGet2D(image, i, j);
			scalar1.val[0] = newPixelValue1;
			cvSet2D(image, i, j, scalar1);

			CvScalar scalar2 = cvGet2D(image, i + 1, j);
			scalar2.val[0] = newPixelValue2;
			cvSet2D(image, i + 1, j, scalar2);

			cvSaveImage("newim.jpg", image, 0);

			count += 2;
			if (count == MESSAGE_LENGTH)
				flag = 1;

		}
	}


}