#ifndef _INC_IMAGEIO
#define _INC_IMAGEIO

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class ImageIO {
    private :
        Mat image;

    public:
        ImageIO();
        ImageIO(char *filename);
        ImageIO(Mat mat_image);
        int set_image_by_filename(char *filename);
        int set_image_by_mat(Mat mat_image);
        int display_image(int wait_time = 0);
        Mat get_image();
        Mat get_grayscale_image();
};

#endif
