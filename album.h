#ifndef ALBUM
#define ALBUM

#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

const int UpTo = 50;
typedef char String[UpTo];

class Album
{
    public :
        Album (const Album& otherAlbum);
        Album (String upc, String artist,
                    String title);
        Album ();
        Album & operator = (const Album& otherAlbum);
        friend bool operator != (const Album& thisAlbum, const Album& otherAlbum);
        friend bool operator == (const Album& thisAlbum, const Album& otherAlbum);
        friend bool operator < (const Album& thisAlbum, const Album& otherAlbum);
        friend bool operator > (const Album& thisAlbum, const Album& otherAlbum);
        friend istream & operator >> (istream & stream, Album & C);
        friend ostream & operator << (ostream & stream, Album & C);
        
        string getUPC () const;
        int recordSize() const;

    private :
        String UPC, Artist, Title;
};
#endif
