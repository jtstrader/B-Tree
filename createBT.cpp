#include<iostream>
#include<fstream>
#include "album.h"
#include "btree.h"

using namespace std;

bool checkArgs(char* inputFileName, char* outputFileName);

int main(int argc, char** argv) {

    // no args passed
    if(argc==1) {
        cout<<"createBT: Invalid syntax! Use 'myCreate --help' for more information."<<endl;
        return 1;
    }

    if(strcmp("--help", argv[1]) == 0) {
        system("cat help_create");
	    return 0;
    }

    else if(argc!=3) {
        cout<<"createBT: Invalid syntax! Use 'myCreate --help' for more information."<<endl;
        return 1;
    }

    char* inputFileName = argv[1];
    char* outputFileName = argv[2];


    // check args for errors. return 1 (failure) if file(s) not found
    if(!checkArgs(inputFileName, outputFileName))
        return 1;

    fstream inFile(inputFileName, ios::in);
    BTree newBT; Album input;

    // write header information for the new BTree
    newBT.reset(outputFileName);
    newBT.writeHeader(outputFileName);
    while(inFile>>input)
        newBT.insert(input);
    newBT.printTree();
    newBT.close();
}

bool checkArgs(char* inputFileName, char* outputFileName) {
    fstream inTest(inputFileName, ios::in);
    fstream outTest(outputFileName, ios::in);

    bool open = true;
    if(!(inTest.is_open())) {
	cout<<"createBT: File "<<inputFileName<<" could not be found."<<endl;
        open = false;
    }
    if(!(outTest.is_open())) {
        cout<<"createBT: File "<<outputFileName<<" could not be found."<<endl;
        
        // special case: create new tree file
        cout<<"Create new file? [Y/N]: ";
        string input; cin>>input;
        char ans = input[0]; // catch errors        

        if(tolower(ans)=='y') {
	    fstream createNewTreeFile(outputFileName, ios::out | ios::trunc);
            createNewTreeFile.close();
        }
        else
            open = false;
    }

    inTest.close();
    outTest.close();
    return open;
}
