all: big-two

big-two: big-two.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(all)

