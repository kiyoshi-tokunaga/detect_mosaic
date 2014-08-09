main: ImageIO.o main.o
	g++ -Wall `pkg-config --cflags opencv` `pkg-config --libs opencv` ImageIO.o main.o -o main
ImageIO.o: ./lib/ImageIO.cpp
	g++ -Wall -c -I./include ./lib/ImageIO.cpp
main.o: ./main.cpp
	g++ -Wall -c ./main.cpp
clean:
	rm -f *.o main
