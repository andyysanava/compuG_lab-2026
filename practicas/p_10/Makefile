CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I"External Libraries/glm" -I"External Libraries"

ASSIMP_CFLAGS = $(shell pkg-config --cflags assimp)
ASSIMP_LIBS   = $(shell pkg-config --libs assimp)

LIBS = -lGL -lGLEW -lglfw $(ASSIMP_LIBS) -ldl -lpthread

SOIL2_SOURCES = $(wildcard External\ Libraries/SOIL2/*.c)
SOIL2_OBJECTS = $(SOIL2_SOURCES:.c=.o)

all: Iluminacion

Iluminacion: Iluminacion.o $(SOIL2_OBJECTS)
	$(CXX) $(CXXFLAGS) $(ASSIMP_CFLAGS) -o $@ $^ $(LIBS)

Iluminacion.o: Iluminacion.cpp
	$(CXX) $(CXXFLAGS) $(ASSIMP_CFLAGS) -c -o $@ $<

%.o: %.c
	gcc -c -o $@ $<

clean:
	rm -f Iluminacion.o $(SOIL2_OBJECTS) Iluminacion
