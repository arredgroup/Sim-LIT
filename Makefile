all:
	g++ -o simlit main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11 -Wall
