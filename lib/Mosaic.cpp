#include "Mosaic.h"
#include "Labeling.h"

cv::RNG rnd(1192);
cv::Scalar randomColor()
{
    return cv::Scalar(rnd.next() & 0xFF, rnd.next() & 0xFF, rnd.next() & 0xFF);
}

bool is_squere(int size, CvPoint left_top, CvPoint right_bottom, float threshold) {
    float gradient;
    gradient = (right_bottom.y - left_top.y)/(right_bottom.x - left_top.x);
    int expectecd_size = (right_bottom.y - left_top.y)*(right_bottom.x - left_top.x);
    if((gradient > 1.0 - threshold) 
            && (gradient < 1.0 + threshold)
            && (size > expectecd_size * (1.0 - threshold))
            && (size < expectecd_size * (1.0 + threshold)))
        return true;
    return false;
}

Mosaic::Mosaic(char *filename) {
    image = imread(filename);
    if(image.empty()){
        cout << "[error]:failed constructingd instance.\n";
    }
}

Mosaic::Mosaic(Mat mat_image) {
    image = mat_image.clone();
    if(image.empty()){
        cout << "[error]:failed constructingd instance.\n";
    }
}

Mat Mosaic::convert_mat(int type) {
    if( image.empty() )
    {
        cout << "Couldn't load " << endl;
    }
    switch(type) {
        break; case MOSAIC_INPUT: { return image; }
        break; case MOSAIC_GRAY: { 
                    Mat grayscale_image;
                    cvtColor(image, grayscale_image, CV_BGR2GRAY);
                    return grayscale_image; 
                }
        break; case MOSAIC_EDGE: {
                    Mat grayscale_image, edge_image;
                    cvtColor(image, grayscale_image, CV_BGR2GRAY);
	                Canny(grayscale_image, edge_image, 5, 50);
                    return edge_image;
                }
        break; case MOSAIC_GRID: { return image; }
        break; default: { return image; }
    }
}

int Mosaic::display_image(int type, int wait_time) {
    Mat responce_image = convert_mat(type);
    if(responce_image.empty()){
        cout << "[error]:cannot display grayscale_images.\n";
        return -1;
    }
    namedWindow("display", WINDOW_AUTOSIZE);
    imshow("display", responce_image);
    waitKey(wait_time);
    return 0;
}

void Mosaic::detect_block() {

    blocks.clear();

    Mat gray = convert_mat(MOSAIC_GRAY);
    Mat edge = convert_mat(MOSAIC_EDGE);
    Mat dilate_dst = edge.clone();
    dilate(edge, dilate_dst, cv::Mat(), cv::Point(-1,-1), 1);
    Mat erode_dst = dilate_dst.clone();
    erode(dilate_dst, erode_dst, cv::Mat(), cv::Point(-1,-1), 1);

    Mat not_dilate = ~dilate_dst;
    Mat not_erode= ~erode_dst;
    Mat label(edge.size(), CV_16SC1);
    LabelingBS  labeling;
    labeling.Exec(not_erode.data, (short *)label.data, edge.cols, edge.rows, true, 50);  
    Mat outimg(edge.size(), CV_8UC3, cv::Scalar(255, 255, 255)); 
    for( int i = 0; i < labeling.GetNumOfResultRegions(); i++)
    {
        RegionInfoBS *ri = labeling.GetResultRegionInfo(i);
        int size = ri->GetNumOfPixels();
        cout << "--------------" << endl;
        cout << "num: " << ri->GetResult() << endl;
        cout << "pixels: " << size << endl;

        // eliminate big size blocks
        if(ri->GetNumOfPixels() > 250)
            continue;

        CvPoint left_top, right_bottom;
        ri->GetMin(left_top.x, left_top.y);
        ri->GetMax(right_bottom.x, right_bottom.y);
        cout << left_top.x << " " << left_top.y << endl;
        cout << right_bottom.x << " " << right_bottom.y << endl;
        cout << (is_squere(size, left_top, right_bottom, 0.3) ? 1 : 0) << endl;

        // eliminate not square blocks
        if(!is_squere(size, left_top, right_bottom, 0.3))
            continue;

        // push expect mosaic blocks
        Block block;
        block.size = size;
        block.left_top = left_top;
        block.right_bottom = right_bottom;
        // get_color
        float gravity_x, gravity_y;
        ri->GetCenterOfGravity(gravity_x, gravity_y);
        CvPoint gravity = cvPoint((int)gravity_x, (int)gravity_y);
        block.BGR[0] = image.at<Vec3b>(gravity.y, gravity.x)[0];
        block.BGR[1] = image.at<Vec3b>(gravity.y, gravity.x)[1];
        block.BGR[2] = image.at<Vec3b>(gravity.y, gravity.x)[2];
        block.intensity = gray.at<unsigned char>(gravity.y, gravity.x);
        blocks.push_back(block);
        cout << block.BGR[0] << ", "
            << block.BGR[1] << ", " 
            << block.BGR[2] <<  ", "
            << block.intensity << endl;
        cout << "come here." << endl;

        //TODO: klustering from color, position, voting

        // visiable result of labeling
        Mat labelarea;
        compare(label, i + 1, labelarea, CV_CMP_EQ);
        Mat color(edge.size(), CV_8UC3, randomColor());
        color.copyTo(outimg, labelarea);
    }

    namedWindow("display", WINDOW_AUTOSIZE);
    imshow("display", outimg);
    namedWindow("display_color", WINDOW_AUTOSIZE);
    imshow("display_color", convert_mat(MOSAIC_INPUT));
    waitKey(10000);
}
