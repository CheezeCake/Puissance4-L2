CC=g++
CFLAGS=-O2 -W -Wall -pedantic
LIBS=`pkg-config --libs sdl` -lSDL_image -lSDL_ttf
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
TARGET=Game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o bin/$@ $(LIBS)

$(OBJ): %.o: %.cpp
	$(CC) -c $^ $(CFLAGS)

run: $(TARGET)
	cd ./bin && ./$(TARGET)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf bin/$(TARGET)
