
default: build

main: *.?pp sdl/*.?pp
	$(CXX) -I. -I/usr/include/SDL/ -lSDL -o main main.cpp

build: main

.PHONY: default build
