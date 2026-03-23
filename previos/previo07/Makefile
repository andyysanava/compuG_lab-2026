LDFLAGS = -lglfw -lGL -lGLEW -lpthread -lXi -ldl -lm -lassimp
INC = -Iinclude -ISOIL2

SOIL2_SOURCES = $(wildcard SOIL2/*.c)
SOIL2_OBJECTS = $(SOIL2_SOURCES:.c=.o)

SOURCES = $(wildcard *.cpp)
TARGETS = $(SOURCES:.cpp=.out)

all: $(TARGETS)

%.out: %.cpp $(SOIL2_OBJECTS)
	g++ $(INC) $< $(SOIL2_OBJECTS) -o $@ $(LDFLAGS)

SOIL2/%.o: SOIL2/%.c
	gcc -c $< -o $@ $(INC)

clean:
	rm -f *.o SOIL2/*.o *.out
