#include <iostream>
#include "include/Mosaic.h"

using namespace std;

int main (int argc, char *argv[]) {
    if(argc != 2){
        cout << "[error]:inputed unread image path.";
        return -1;
    }
    Mosaic *mosaic = new Mosaic(argv[1]);
    //mosaic->display_grayscale_image(1000);
    //mosaic->display_edge_image(10000);
    mosaic->detect_block();
    return 0;
}
