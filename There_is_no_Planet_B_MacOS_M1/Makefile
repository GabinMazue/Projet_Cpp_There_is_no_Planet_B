CC=g++
CCFLAGS= -Wall -Werror -std=c++11 -g
LIBFLAGS= -L./lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
SRC= $(wildcard src/*.cc)
OBJ= $(SRC:.cc=.o)
TST_DIR= tests/
TST= $(wildcard $(TST_DIR)/*.cc)
OBJ_TEST = $(filter-out main.o, $(OBJ)) $(TST:.cc=.o)
EXEC=jeu


all: $(EXEC)

testcase :
	cd $(TST_DIR) ;  make

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LIBFLAGS)

%.o: %.cc
	$(CC) $(CCFLAGS) -o $@ -c $< -I./include

.depend:
	g++ $(CCFLAGS) -MM $(SRC) > .depends
-include .depends

clean:
	rm -f $(OBJ) $(EXEC)
cleantest:
	cd $(TST_DIR) ; make clean
distclean : clean cleantest
