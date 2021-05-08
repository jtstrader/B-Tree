#include "album.h"

// default constructor
Album::Album() {}

// copy constructor
Album::Album(const Album& otherAlbum) {
    strcpy(UPC, otherAlbum.UPC);
    strcpy(Artist, otherAlbum.Artist);
    strcpy(Title, otherAlbum.Title);
}

// returns dereferenced *this Album with copied information
Album & Album::operator = (const Album& otherAlbum) {
    strcpy(UPC, otherAlbum.UPC);
    strcpy(Artist, otherAlbum.Artist);
    strcpy(Title, otherAlbum.Title);
    return *this;
}

// returns if the two Albums are equal to each other
bool operator == (const Album& thisAlbum, const Album& otherAlbum) {
    return thisAlbum.UPC == otherAlbum.UPC;
}

bool operator != (const Album& thisAlbum, const Album& otherAlbum) {
    return thisAlbum.UPC != otherAlbum.UPC;
}

// less than operator for general comparison
// only compares the UPCs of the two albums
bool operator < (const Album& thisAlbum, const Album& otherAlbum) {
    return thisAlbum.getUPC() < otherAlbum.getUPC();
}

// greater than operator for general comparison
// only compares the UPCs of the two albums
bool operator > (const Album& thisAlbum, const Album& otherAlbum) {
    return thisAlbum.getUPC() > otherAlbum.getUPC();
}

// input overload allows user to read in entirety of Album
istream& operator >> (istream& stream, Album & C) {
    stream.getline(C.UPC, 50, '\n');
    stream.getline(C.Artist, 50, '\n');
    stream.getline(C.Title, 50, '\n');
    return stream;
}

// output overload that prints entirety of Album to stream
ostream& operator << (ostream& stream, Album & C) {
    return stream<<C.UPC<<"|"<<C.Artist<<"|"<<C.Title;
}

// return private UPC variable to client file
string Album::getUPC() const {
    return UPC;
}

// return the size of the record that is used to create the B-Tree
int Album::recordSize() const {
    return sizeof(Album);
}