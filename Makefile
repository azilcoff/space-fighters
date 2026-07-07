all: build run

build: src/main.cpp
	g++ -Iglad/include -o bin/space-fighters -Wall -std=c++20 src/main.cpp

clean:
	rm bin/space-fighters