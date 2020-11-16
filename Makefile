CC = gcc
CPPFLAGS = -D VERBOSE
CFLAGS = -g -Wall
LDFLAGS = -lpthread 

OBJS = image_ppm.o image_filter_parallel.o main.o 
PROGS = colour_battle 

all: $(PROGS)

colour_battle: $(OBJS)
	$(CC) -o colour_battle $(OBJS) $(LDFLAGS)


.PHONY: clean
clean:
	rm -rf $(PROGS) *.o *.dSYM
