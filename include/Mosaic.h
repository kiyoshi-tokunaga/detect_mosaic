#ifndef _INC_MOSAIC
#define _INC_MOSAIC

#include <math.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


enum MOAIC_TYPE {
    MOSAIC_INPUT = 0,
    MOSAIC_GRAY = 1,
    MOSAIC_EDGE = 2,
    MOSAIC_GRID = 3,
};

struct Block {
    int BGR[3];
    int size;
    int intensity;
    int rigion_num_debug;
    CvPoint left_top;
    CvPoint right_bottom;
    CvPoint gravity;
};

class Mosaic {
    private:
        Mat image;
        vector <Block> blocks;

        Mat convert_mat(int type);

    public:
        Mosaic(char *filename);
        Mosaic(Mat mat_image);
        int display_image(int type = MOSAIC_INPUT, int wait_time = 10000);
        void detect_block();

};

#endif
