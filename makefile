BACK = blobsBack.c blobsBack.h
BACKC = blobsBack.c
FRONT = BlobWars.c
OBJECTS = blobsBack.o BlobWars.o
NAME = BlobWars.game

build: $(OBJECTS)
	gcc -Wall $(OBJECTS) -lm -o $(NAME) 

back.o: $(BACK)
	gcc -c $(BACKC)

front.o: $(FRONT)
	gcc -c $(FRONT)

clean:
	rm -f *.o

full: 
	make && make clean
