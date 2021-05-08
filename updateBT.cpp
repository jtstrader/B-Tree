#include<iostream>
#include<string>
#include "album.h"
#include "btree.h"

using namespace std;

bool checkArgs(char* inputFileName, char* transactionFileName);

int main(int argc, char** argv) {

    if(argc==1) {
        cout<<"updateBT: Invalid syntax! Use 'myUpdate --help' for more information!"<<endl;
        return 1;
    }    

    if(strcmp("--help", argv[1]) == 0) {
        system("cat help_update");
        return 0;
    }

    else if(argc!=3) {
        cout<<"updateBT: Invalid syntax! Use 'myUpdate --help' for more information!"<<endl;
        return 1;
    }


    char* inputFileName = argv[1];
    char* transactionFileName = argv[2];

    // check flags, return 1 (failure) if file(s) could not be opened
    if(!checkArgs(inputFileName, transactionFileName))
        return 1;

    fstream transactionFile(transactionFileName, ios::in);

    BTree readIn; Album inputAlbum;
    readIn.defineRoot(inputFileName);
    char instruct; string key;
    while(transactionFile>>instruct) {
        transactionFile.ignore(1, '\n');
        switch(instruct) {
            case 'I':
                // insert
                transactionFile>>inputAlbum;
                readIn.insert(inputAlbum);
                cout<<"\t"<<"key: "<<inputAlbum<<" added."<<endl;
                break;
            case 'S':
                // search
                getline(transactionFile, key);
                bool found = readIn.search(key);
                cout<<"\t\""<<key<<"\"";
                if(found) cout<<" found"<<endl;
                else cout<<" not found"<<endl;
                break;
        }
    }
    cout<<endl;
    readIn.totalio();

}

bool checkArgs(char* inputFileName, char* transactionFileName) {
    fstream inTest(inputFileName, ios::in);
    fstream transTest(transactionFileName, ios::in);

    bool open = true;
    if(!(inTest.is_open())) {
        cout<<"updateBT: File "<<inputFileName<<" could not be found."<<endl;
        open = false;
    }
    if(!(transTest.is_open())) {
        cout<<"updateBT: File "<<transactionFileName<<" could not be found."<<endl;
        open = false;
    }

    inTest.close();
    transTest.close();
    return open;
}

