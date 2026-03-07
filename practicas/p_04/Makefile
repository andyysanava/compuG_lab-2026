CPPFLAGS=-lglfw -lGL -lGLEW -lpthread -lXi -ldl -lm 
INC=-Iinclude

%: %.cpp
	g++ $(CPPFLAGS) $(INC) $< -o $@.out

%: %.c
	cc $(CPPFLAGS) $(INC) $< -o $@.out
