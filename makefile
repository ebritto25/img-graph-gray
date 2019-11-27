# My third makefile

# Name of the project
PROJ_NAME=graph_descriptor

# .c files
CPP_SOURCE=$(wildcard ./src/*.cpp)

# .h files
HPP_SOURCE=$(wildcard ./src/*.hpp)

# Object files
OBJ=$(subst .cpp,.o,$(subst src,objects,$(CPP_SOURCE)))

# Compiler and linker
CC=g++

INCLUDES=-I/usr/include/boost

LIBS=$(shell pkg-config --libs opencv) \
	  -ligraph                         \
	  -lboost_system                   \
	  -lboost_filesystem               \
	  -lpthread 

LFLAGS=-L/usr/local/lib 




# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall      \
         -ansi      \
         -pedantic  \
		 -std=c++17 \
		 -O3        

CV_FLAGS=$(shell pkg-config --cflags opencv)

# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using G++ linker: $@'
	$(CC)  $(CV_FLAGS) $(INCLUDES) $^ -o $@ $(LFLAGS) $(LIBS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./src/%.cpp ./src/%.hpp
	@ echo 'Building target using G++ compiler: $<'
	$(CC)  $< $(CC_FLAGS)  $(CV_FLAGS) $(INCLUDES) -o $@ 
	@ echo ' '

./objects/%.o: ./src/%.cpp 
	@ echo 'Building target using G++ compiler: $<'
	$(CC)  $< $(CC_FLAGS)  $(CV_FLAGS) $(INCLUDES) -o $@ 
	@ echo ' '


./objects/main.o: ./src/main.cpp $(HPP_SOURCE)
	@ echo 'Building target using G++ compiler: $<'
	$(CC)  $< $(CC_FLAGS)  $(CV_FLAGS) $(INCLUDES) -o $@ 
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean
