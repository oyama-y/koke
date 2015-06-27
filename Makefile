CC = gcc
CFLAGS = -O3
LFLAGS = 
KOKE = koke

$(KOKE): main.o
	$(CC) $(LFLAGS) $^ -o $(KOKE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $*.o

clean:
	rm -f $(KOKE)
	rm -f *.o
