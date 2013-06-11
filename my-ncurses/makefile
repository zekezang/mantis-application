CFLAGS := -I/sourcecode/arm-workspace/my-ncurses/include
CFLAGS += -L/sourcecode/arm-workspace/my-ncurses/lib
CFLAGS += -lncurses -lncurses_g

a : test.o
	gcc -o a test.o $(CFLAGS)
test.o : ./source/test.c
	gcc -c ./source/test.c $(CFLAGS)