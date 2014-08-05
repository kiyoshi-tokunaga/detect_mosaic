#include"mosaic_block.h"

int main (int argc, char *argv[]) {
    MosaicBlock block;
    char *filename;
    if(argc == 2){
        block.read_image(argv[1]);
    }else{
        printf("[error]:inputed unread image path.");
        return -1;
    }
    block.display_image();
    return 0;
}

MosaicBlock::MosaicBlock(){

}

int MosaicBlock::read_image(char *filename) {
    MosaicBlock::image = imread(filename);
    if(filename == NULL){
        printf("[error]:cannot read %s.\n", filename);
        return -1;
    }
    return 0;
}

int MosaicBlock::display_image(int wait_time) {
    if(image.empty()){
        printf("[error]:cannot display images.\n");
        return -1;
    }
    namedWindow("display image", WINDOW_AUTOSIZE);
    imshow("display image", image);
    waitKey(wait_time);
    return 0;
}
