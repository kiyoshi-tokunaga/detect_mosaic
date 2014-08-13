main: ImageIO.o Mosaic.o main.o
	g++ -Wall `pkg-config --cflags opencv` `pkg-config --libs opencv` ImageIO.o  Mosaic.o main.o -o main
ImageIO.o: ./lib/ImageIO.cpp
	g++ -Wall -c -I./include ./lib/ImageIO.cpp
Mosaic.o: ./lib/Mosaic.cpp
	g++ -Wall -c -I./include ./lib/Mosaic.cpp
main.o: ./main.cpp
	g++ -Wall -c ./main.cpp
clean:
	rm -f *.o main
