
CFLAGS = -g -Wall -W
LDFLAGS =

EXE = aoc
OBJS = aoc.o


$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^


%.o: %.c
	$(CC) $(CFLAGS) -c $<


.PHONY: clean

clean:
	rm -f $(OBJS) $(EXE)
