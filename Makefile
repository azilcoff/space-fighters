all: build run

build: src/main.cpp
	g++ -Iinclude -Iexternal/glad/include -o bin/space-fighters -Wall -std=c++20 src/main.cpp external/glad/src/glad.c -lglfw

run: bin/space-fighters
	./bin/space-fighters

clean:
	rm bin/space-fighters