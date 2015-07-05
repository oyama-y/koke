CC = gcc
CFLAGS = -O3 -Wall
LFLAGS = -Wall
KOKE = koke

$(KOKE): main.o gamedata.o koke.o
	$(CC) $(LFLAGS) $^ -o $(KOKE)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $*.o

clean:
	rm -f $(KOKE)
	rm -f *.o
