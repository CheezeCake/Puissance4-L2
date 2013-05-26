CXX=g++
CXXFLAGS=-O2 -W -Wall -pedantic
LIBS=`pkg-config --libs sdl` -lSDL_image -lSDL_ttf
SRCDIR=src/
OBJDIR=obj/
SRC=$(wildcard $(SRCDIR)*.cpp)
OBJ=$(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRC))
INCLUDEDIR=include/
BINDIR=bin/
TARGET=Game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $(BINDIR)$@ $(LIBS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -c $^ -o $@ -I $(INCLUDEDIR) $(CXXFLAGS)

.PHONY: clean mrproper run

run: $(TARGET)
	cd $(BINDIR) && ./$(TARGET)

clean:
	@rm -f $(OBJDIR)*.o

mrproper: clean
	@rm -f $(BINDIR)$(TARGET)
