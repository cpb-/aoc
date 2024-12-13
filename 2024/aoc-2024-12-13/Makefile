
CFLAGS = -g -Wall -W
LDFLAGS =

EXE = aoc
OBJS = aoc.o


$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm


%.o: %.c
	$(CC) $(CFLAGS) -c $<


.PHONY: clean

clean:
	rm -f $(OBJS) $(EXE)
