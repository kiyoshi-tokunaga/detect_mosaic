#ifndef _INC_IMAGEIO
#define _INC_IMAGEIO

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class ImageIO {
    private :
        Mat image;

    public:
        ImageIO();
        int read_image(char *filename);
        int display_image(int wait_time = 0);
};

#endif
