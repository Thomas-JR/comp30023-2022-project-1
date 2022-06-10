CC=gcc
CFLAGS=-Wall
EXE=detect
OBJ=detect.o tasks.o tools.o

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $^

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(EXE)

format:
	clang-format -i *.c *.h