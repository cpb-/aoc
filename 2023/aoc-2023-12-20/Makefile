
CFLAGS = -g -Wall -W
LDFLAGS = -laoc -L.

EXE = aoc
OBJS = aoc.o
LIBS = libaoc.a

$(EXE): $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^

libaoc.a: libaoc.o
	$(AR) -rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<


.PHONY: clean

clean:
	rm -f $(OBJS) $(EXE) $(LIBS) *.o
