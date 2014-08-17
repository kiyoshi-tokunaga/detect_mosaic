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

bool calc_simirarity(Block, Block, int, float);

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

    vector <Block> squares;
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

    for( int i = 0, n = labeling.GetNumOfResultRegions(); i < n; i++){
        RegionInfoBS *ri = labeling.GetResultRegionInfo(i);
        int size = ri->GetNumOfPixels();

        // eliminate big size blocks
        if(ri->GetNumOfPixels() > 250)
            continue;

        CvPoint left_top, right_bottom;
        ri->GetMin(left_top.x, left_top.y);
        ri->GetMax(right_bottom.x, right_bottom.y);

        // eliminate not square blocks
        if(!is_squere(size, left_top, right_bottom, 0.3))
            continue;

        // push expect mosaic blocks
        Block block;
        block.size = size;
        block.left_top = left_top;
        block.right_bottom = right_bottom;
        // for visialize
        block.rigion_num_debug = i;
        // get_color
        float gravity_x, gravity_y;
        ri->GetCenterOfGravity(gravity_x, gravity_y);
        CvPoint gravity = cvPoint((int)gravity_x, (int)gravity_y);
        block.gravity = gravity;
        block.BGR[0] = image.at<Vec3b>(gravity.y, gravity.x)[0];
        block.BGR[1] = image.at<Vec3b>(gravity.y, gravity.x)[1];
        block.BGR[2] = image.at<Vec3b>(gravity.y, gravity.x)[2];
        block.intensity = gray.at<unsigned char>(gravity.y, gravity.x);
        squares.push_back(block);

        // visiable result of labeling
        /// Mat labelarea;
        /// compare(label, i + 1, labelarea, CV_CMP_EQ);
        /// Mat color(edge.size(), CV_8UC3, randomColor());
        /// color.copyTo(outimg, labelarea);
    }

    // pick mosaic block
    vector <bool>  is_chain;
    is_chain.resize(squares.size(), false);
    float threshold = 0.8;
    int distance_threshold = image.cols < image.rows
        ? image.rows / 3
        : image.cols / 3;
    for(int i = 0, n = squares.size(); i < n; i++){
        for(int j = i + 1, m = squares.size(); j < m; j++){
            if(calc_simirarity(squares[i], squares[j], distance_threshold, threshold)){
                is_chain[i] = true;
            }
        }
    }
    for(int i = 0, n = squares.size(); i < n; i++){
        if(is_chain[i])
            blocks.push_back(squares[i]);
    }

    // result visialize
    for(int i = 0, n = blocks.size(); i < n; i++){
        // visiable result of labeling
        Mat labelarea;
        compare(label, blocks[i].rigion_num_debug + 1, labelarea, CV_CMP_EQ);
        Mat color(edge.size(), CV_8UC3, randomColor());
        color.copyTo(outimg, labelarea);
    }

    namedWindow("display", WINDOW_AUTOSIZE);
    imshow("display", outimg);
    namedWindow("display_color", WINDOW_AUTOSIZE);
    imshow("display_color", convert_mat(MOSAIC_INPUT));
    waitKey(10000);
}

bool calc_simirarity(const Block prev, const Block next, const int distance_threshold, float threshold){
    float area_simirarity = prev.size < next.size
        ? (float)prev.size / next.size 
        : (float)next.size / prev.size;
    float color_simirarity =  1.0 -
        (float)(abs(prev.BGR[0] - next.BGR[0]) 
         + abs(prev.BGR[1] - next.BGR[1]) 
         + abs(prev.BGR[2] - next.BGR[2])) 
         / (3.0*256); 
    double gravity_distance = 
        sqrt((prev.gravity.x-next.gravity.x)*(prev.gravity.x-next.gravity.x) 
            + (prev.gravity.y-next.gravity.y)*(prev.gravity.y-next.gravity.y));
    float gravity_simirarity = 
        gravity_distance < distance_threshold 
        ? 1.0 - gravity_distance/distance_threshold
        : 0;
    if(area_simirarity < threshold
            || color_simirarity < threshold
            || gravity_simirarity < threshold){
        return false;
    }
    return true;
}
