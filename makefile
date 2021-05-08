all: create update

create: createBT.cpp btree.o album.o
	c++ -o myCreate createBT.cpp btree.o album.o

update: updateBT.cpp btree.o album.o
	c++ -o myUpdate updateBT.cpp btree.o album.o

btree.o: btree.cpp
	c++ -c btree.cpp

album.o: album.cpp
	c++ -c album.cpp

clean:
	rm -f *.o
