#ifndef _INC_MOSAIC_BLOCK
#define _INC_MOSAIC_BLOCK

#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class MosaicBlock {
    private :
        Mat image;

    public:
        MosaicBlock();
        int read_image(char *filename);
        int display_image(int wait_time = 0);
};

#endif
