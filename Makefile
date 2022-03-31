all:
	@g++ -std=c++11 main.cpp -lglfw3 -lGLU -lGL -lX11 -pthread -lSDL2main -lSDL2 