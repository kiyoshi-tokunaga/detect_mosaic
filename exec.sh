#!/usr/local/bin/zsh
g++ `pkg-config --cflags opencv` `pkg-config --libs opencv` -I./include ./lib/mosaic_block.cpp 
./a.out ./image/test.jpg
