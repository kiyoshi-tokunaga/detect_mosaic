#include "ImageIO.h"
#include <iostream>

ImageIO::ImageIO(){

}

int ImageIO::read_image(char *filename) {
    ImageIO::image = imread(filename);
    if(filename == NULL){
        cout << "[error]:cannot read.\n";
        return -1;
    }
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
