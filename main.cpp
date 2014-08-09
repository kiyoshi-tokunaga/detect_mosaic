#include <iostream>
#include "./include/ImageIO.h"

using namespace std;

int main (int argc, char *argv[]) {
    ImageIO image;
    char *filename;
    if(argc == 2){
        image.read_image(argv[1]);
    }else{
        cout << "[error]:inputed unread image path.";
        return -1;
    }
    image.display_image();
    return 0;
}
