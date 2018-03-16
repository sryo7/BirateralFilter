#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

void bilateralfilter(int kernelsize,int sigma_distance, int sigma_gap);

Mat src = imread("arimura.png", 1);
Mat dst = src.clone();

int main(int argc, char *argv[]) {
	imshow("origin", src);
	bilateralfilter(5,8,16);
	imshow("result", dst);
	imwrite("result.png", dst);
	waitKey(0);

	return 0;
}

void bilateralfilter(int kernelsize,int sigma_distance, int sigma_gap) {
    double numerator = 0;
	double denominator = 0;
	int margin = (int)(kernelsize/2);

	for (int c = 0; c < src.channels(); ++c) {
		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				for (int i = 0; i < kernelsize; i++) {
					for (int j = 0; j < kernelsize; j++) {
						if (y - margin+j < 0 || y - margin + j > src.rows ||
							x - margin+i< 0 || x - margin + i > src.cols) {
							continue;
						} else {
							numerator += src.data[(y - margin + j) * src.step + (x - margin + i)* src.elemSize() + c] 
										* exp(-(i*i + j*j) / (2 * sigma_distance * sigma_distance))
										* exp(-pow(src.data[y*src.step + x * src.elemSize() + c] - src.data[(y - margin+ j)
									    * src.step + (x - margin + i) * src.elemSize() + c], 2) /( 2 * pow(sigma_gap, 2)));

							denominator += exp(-(i*i + j*j) / (2 * sigma_distance * sigma_distance))
								* exp(-pow(src.data[y*src.step + x * src.elemSize() + c] - src.data[(y - margin + j)
									* src.step + (x - margin + i) * src.elemSize() + c], 2) / (2 * pow(sigma_gap, 2)));
						}
					}
				}

			    dst.data[y*dst.step + x * dst.elemSize() + c] = (int)(numerator / denominator);
				numerator = 0;
				denominator = 0;

			}
		}

	}
}