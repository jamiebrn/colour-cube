CPP := g++
CPPFLAGS := -I include/ -L lib/
LIBFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC_FILES := src/*.cpp

EXEC := main.exe

all:
	$(CPP) $(SRC_FILES) -o bin/$(EXEC) -O3 $(CPPFLAGS) $(LIBFLAGS)

run:
	cd bin && ./$(EXEC)
