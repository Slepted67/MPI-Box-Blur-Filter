#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

extern "C" int* imageToMat(char* filename, int* dims) {
    Mat image = imread(filename, ISMREAD_COLOR);
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);

    if (!image.data) {
        cout << "Could not open or find the image" << endl;
        return nullptr;
    }

    int width = gray_image.cols;
    int height = gray_image.rows;
    dims[0] = height;
    dims[1] = width;

    int* matrix = (int*)malloc(height * width * sizeof(int));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            matrix[i * width + j] = gray_image.at<uchar>(i, j);

    return matrix;
}

extern "C" void matToImage(char* filename, int* mat, int* dims) {
    int height = dims[0];
    int width = dims[1];
    Mat image(height, width, CV_8UC1);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            image.at<uchar>(i, j) = mat[i * width + j];

    imwrite(filename, image);
}
