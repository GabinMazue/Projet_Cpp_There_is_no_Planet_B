CC=g++
CCFLAGS= -Wall -Werror -std=c++11 -g
LIBFLAGS= -L../lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
SRC= $(wildcard ../src/*.cc)
OBJ= $(SRC:.cc=.o)
TST= $(wildcard *.cc)
OBJ_TEST = $(filter-out ../src/jeu.o, $(OBJ)) $(TST:.cc=.o)

testcase : $(OBJ_TEST)
	$(CC) $(CCFLAGS)  -o $@ $^ $(LIBFLAGS)

%.o: %.cc
	$(CC) $(CCFLAGS) -I../include -o $@ -c $<

clean :
	rm -f $(OBJ_TEST) testcase
