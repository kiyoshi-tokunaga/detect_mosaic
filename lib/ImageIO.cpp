#include "ImageIO.h"
#include <iostream>

ImageIO::ImageIO() {

}

ImageIO::ImageIO(char *filename) {
    image = imread(filename);
    if(image.empty()){
        cout << "[error]:failed constructingd instance.\n";
    }
}

ImageIO::ImageIO(Mat mat_image) {
    image = mat_image.clone();
    if(image.empty()){
        cout << "[error]:failed constructingd instance.\n";
    }
}

int ImageIO::set_image_by_filename(char *filename) {
    ImageIO::image = imread(filename);
    if(filename == NULL){
        cout << "[error]:cannot read.\n";
        return -1;
    }
    return 0;
}

int ImageIO::set_image_by_mat(Mat mat_image) {
    if(mat_image.empty()){
        cout << "[error]:cannot read.\n";
        return -1;
    }
    image = mat_image;
    return 0;
}

int ImageIO::display_image(int wait_time) {
    if(image.empty()){
        cout << "[error]:cannot display images.\n";
        return -1;
    }
    namedWindow("display image", WINDOW_AUTOSIZE);
    imshow("display image", image);
    waitKey(wait_time);
    return 0;
}

Mat ImageIO::get_grayscale_image() {
    Mat grayscale_image;
    if(image.empty()){
        cout << "[error]:cannot display images.\n";
        return grayscale_image;
    }
    cvtColor(image, grayscale_image, CV_BGR2GRAY);
    return grayscale_image;
}
