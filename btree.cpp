#include "btree.h"

// PUBLIC FUNCTIONS //
BTree::BTree() {
    // define variables
    rootAddr = -1;
    height = 0; read = 0; write = 0;
}

void BTree::writeHeader(char* fileName) {
    // read into file
    treeFile.open(fileName, ios::in | ios::out | ios::binary);

    // write dummy with header information
    BTNode dummy; dummy.child[0] = sizeof(BTNode);
    treeFile.write((char*)&dummy, sizeof(BTNode));
    write++;

    // define root information
    rootAddr = sizeof(BTNode);
    root.currSize = 0;
    for(int i=0; i<ORDER; i++)
        root.child[i] = -1;
}

void BTree::defineRoot(char* fileName) {
    // defines the header for reading in special
    // transactions or re-reading a B-Tree previously
    // defined
    
    treeFile.open(fileName, ios::in | ios::out | ios::binary);
    BTNode dummy; treeFile.read((char*)&dummy, sizeof(BTNode));
    rootAddr = dummy.child[0];
    root = getNode(dummy.child[0]);
    read++;
}

void BTree::insert(keyType key) {
    // insert a key into the BTree
    // 2 cases:
    //   1. BTree is empty. Insert into root
    //   2. BTree is not empty. Run general algorithm

    cout<<"Now inserting "<<key<<endl;

    // case 1: empty BTree
    if(root.currSize==0) {
        root.contents[0] = key;
        root.currSize++;
        treeFile.seekp(rootAddr);
        treeFile.write((char*)&root, sizeof(BTNode));
        write++;
    }

    // case 2: general case
    else {
        // steps:
        //    1. gather BTNode using getNode(int recAddr)
        //    2. call on private method
        
        int recAddr = findAddr(key, root, rootAddr);
        insert(key, recAddr);
    }
}

void BTree::reset(char* fileName) {
    if(treeFile.is_open())
        treeFile.close();
    treeFile.open(fileName, ios::out | ios::trunc); // truncate option clears file
    treeFile.close();
}

void BTree::close() {
    treeFile.close();
}

void BTree::printTree() {
    cout<<"======== B-Tree of height "<<height<<" ========"<<endl;
    printTree(rootAddr);
}

void BTree::inorder() {
    inorder(rootAddr);
}

void BTree::reverse() {
    reverse(rootAddr);
}

int BTree::getHeight() {
    return height;
}

bool BTree::search(string key) {
    return search(key, root, rootAddr);
}

keyType BTree::retrieve(string key) {
    return retrieve(key, root, rootAddr);
}

void BTree::totalio() const {
    cout<<"Read: "<<read<<endl;
    cout<<"Write: "<<write<<endl;
}

int BTree::countLeaves() {
    return countLeaves(rootAddr);
}

//////////////////////


// PRIVATE FUNCTIONS //

// credit to Dr. Digh for this method
void BTree::printTree(int recAddr)
{
    if(recAddr!=-1) {
        BTNode dummy = getNode(recAddr);
        printNode(recAddr);
        for(int i=0;  i<=dummy.currSize; i++)
            printTree(dummy.child[i]);
    }
}

void BTree::inorder(int rootAddr) {
    // print inorder traversal of the tree
    // prints the tree in order from smallest to largest
    if(rootAddr == -1) return;
    BTNode dummy = getNode(rootAddr);
    int i;
    for(i=0; i<dummy.currSize; i++) {
        inorder(dummy.child[i]);
        cout<<dummy.contents[i]<<endl;
    }
    inorder(dummy.child[i]); // i == currSize
}

void BTree::reverse(int rootAddr) {
    // print the reverse of the tree
    // prints the tree in order from largest to smallest
    if(rootAddr == -1) return;
    BTNode dummy = getNode(rootAddr);
    int i;
    for(i=dummy.currSize; i>=0; i--) {
        if(i<dummy.currSize)
            cout<<dummy.contents[i]<<endl;
        reverse(dummy.child[i]);
    }
}

int BTree::findAddr(keyType key, BTNode t, int tAddr) {
    // if the root is a leaf, return the current address
    // keys must be inserted into a leaf

    if(isLeaf(t))
        return tAddr;
    
    // otherwise, recursively search through BTNodes and
    // find exactly where the current value belongs

    int pos = 0;
    while(pos<t.currSize && key>t.contents[pos])
        pos++;
    return findAddr(key, getNode(t.child[pos]), t.child[pos]);
}

int BTree::findpAddr(keyType key, BTNode t, int tAddr, int findAddr) {
    // if address you are searching for is the root, return -1
    // root does not have a parent node that can be found
    if(findAddr == rootAddr)
        return -1;

    // search through current node for child in child list. if found, return
    // current address that points to the child (tAddr)

    for(int i=0; i<=t.currSize; i++)
        if(t.child[i]==findAddr)
            return tAddr;

    // otherwise, recursively go through and find which node needs to be investigated next
    int pos = 0;
    while(pos<t.currSize && key>t.contents[pos])
        pos++;
    return findpAddr(key, getNode(t.child[pos]), t.child[pos], findAddr);
}

bool BTree::search(string key, BTNode t, int tAddr) {
    // looks for given key and returns if found
    // takes inspiration from findAddr() method. same principle,
    // different key value being used to search

    int pos = 0;
    while(pos<t.currSize && key>t.contents[pos].getUPC())
        pos++;
    if(t.contents[pos].getUPC()==key) return true; // key found
    if(t.child[pos]==-1) return false; // base case
    return search(key, getNode(t.child[pos]), t.child[pos]);
}

keyType BTree::retrieve(string key, BTNode t, int tAddr) {
    // looks for given key and returns the keyType found
    // takes inspritation from findAddr() method, same principle

    keyType not_found_value;

    int pos = 0;
    while(pos<t.currSize && key>t.contents[pos].getUPC())
        pos++;
    if(t.contents[pos].getUPC()==key) return t.contents[pos]; // key found
    if(t.child[pos]==-1) return not_found_value; // base case
    return retrieve(key, getNode(t.child[pos]), t.child[pos]);
}

void BTree::insert(keyType key, int recAddr) {

    // general layout
    //  1. check if a node needs a split (if BTNode.currSize == ORDER-1)
    //  2. if so, perform a basic split using splitNode()
    //     - recAddr -> initial address and future left address

    BTNode t = getNode(recAddr);

    // cases:
    //  1. node does not need to be split
    //  2. node needs to be split

    // case 1: node does not need to be split
    if(t.currSize<ORDER-1) {
        t.contents[t.currSize++] = key; // add key value to end of list. increment size
        sort(t.contents, t.contents+t.currSize);
        treeFile.seekp(recAddr);
        treeFile.write((char*)&t, sizeof(BTNode));
        write++;
        
        // reflect on the root
        if(recAddr == rootAddr)
            root = t;
    }

    // case 2: node needs to be split
    else {
        int newRecordAddr;
        int rAddr = -1;
        splitNode(key, recAddr, newRecordAddr, rAddr);
    }
}

BTNode BTree::getNode(int recAddr) {
    // return node by seeking to record address and obtained node
    BTNode dummy;
    treeFile.seekg(recAddr);
    treeFile.read((char*)&dummy, sizeof(BTNode));
    treeFile.clear(); // clear error flags if reading went to EOF
    read++;
    return dummy;
}

bool BTree::isLeaf(int recAddr) {
    return isLeaf(getNode(recAddr));
}

bool BTree::isLeaf(BTNode t) {
    return t.child[0]==-1;
}

int BTree::countLeaves(int recAddr) {
    // countes leaves of the tree through recursion
    BTNode dummy = getNode(recAddr);
    if(isLeaf(dummy))
        return 1;
    else {
        int sum = 0;
        for(int i=0; i<=dummy.currSize; i++)
            sum+=countLeaves(dummy.child[i]);
        return sum;
    }
        
}

void BTree::splitNode(keyType& key, int recAddr, int& newRecordAddr, int& rAddr) {
    
    cout<<"Now Splitting!"<<endl;

    // split node process
    //  1. swap out, if necessary, the current key being passed in by reference
    //     with the key at the end of the BTNode.contents[] IF
    //         - BTNode.contents[BTNode.currSize-1] > key
    //  2. sort the node if the swap described above occurs
    //  3. create a new BTNode based on the parent address. newRecordAddr
    //     should be obtained through a call of findpAddr(recAddr);
    //         - if a -1 is returned, this means the root will be adjusted.
    //  4. current record, recAddr, will become the left child. If newRecordAddr
    //     is not the root, then the BTNode for newRecordAddr will not need to be
    //     adjusted
    BTNode currentNode = getNode(recAddr);
    if(isLeaf(currentNode)) {
        if(currentNode.contents[currentNode.currSize-1]>key) {
            // perform step 1 described above
            keyType temp = currentNode.contents[currentNode.currSize-1];
            currentNode.contents[currentNode.currSize-1] = key;
            key = temp;
            sort(currentNode.contents, currentNode.contents+currentNode.currSize);
        }
    }
    else {
        // ricochet splitting is occuring here
        // in this case, rAddr is already defined and is the current excess
        // child data stored in RAM. This data may or may not be swapped out
        // in the child array of the parent node in the case that:
        //     key<currentNode.contents[currentNode.currSize-1]

        // key = key to be investigated if needed to sort
        // recAddr = record address of parent of previous ricochet
        // newRecordAddr = n/a
        // rAddr = address of right child -- tied to the key value
        
        if(currentNode.contents[currentNode.currSize-1]>key) {
            keyType temp = currentNode.contents[currentNode.currSize-1];
            int rAddrTemp = currentNode.child[currentNode.currSize];
            currentNode.currSize--; // lower current size to factor in adding new key value
            
            int shift = 0;
            // grab shift value that determines where the key will be inserted
            while(key > currentNode.contents[shift] && shift < currentNode.currSize)
                shift++;
            int end = currentNode.currSize-1;
            while(end>=shift) { // shift contents over 1 cell each
                currentNode.contents[end+1] = currentNode.contents[end];
                currentNode.child[end+2] = currentNode.child[end+1];
                end--;
            }
            currentNode.contents[shift] = key;
            currentNode.child[shift+1] = rAddr;

            key = temp;
            rAddr = rAddrTemp;
        }
    }
    int pAddr = findpAddr(key, root, rootAddr, recAddr);
    currentNode.currSize = 2;

    BTNode rightNode;
    rightNode.currSize = 2;
    rightNode.contents[0] = currentNode.contents[3];
    rightNode.contents[1] = key; // take max key, this becomes far right value
    rightNode.child[0] = currentNode.child[3];
    rightNode.child[1] = currentNode.child[4];
    rightNode.child[2] = rAddr;
    for(int i=3; i<ORDER; i++)
        rightNode.child[i] = -1;

    treeFile.seekp(recAddr);
    treeFile.write((char*)&currentNode, sizeof(BTNode));

    treeFile.seekp(0, ios::end);
    rAddr = treeFile.tellp();
    treeFile.write((char*)&rightNode, sizeof(BTNode));

    write+=2;
   
    const int mid = ORDER/2;
    if(pAddr==-1) {
        // write out new left and right children, and then place node
        // in a new root location. establish new root and new root header
        adjRoot(currentNode.contents[mid], recAddr, rAddr); // recall recAddr == left child now
    }

    else {
        // place node in level above and call this function once more in the case
        // that the ricochet split needs to occur
        placeNode(currentNode.contents[mid], pAddr, recAddr, rAddr);
    }
}

void BTree::adjRoot(keyType rootElem, int lAddr, int rAddr) {
    // create a new root node, append to the file, and change the header information.
    height++;

    BTNode newRoot;
    newRoot.currSize = 1;
    newRoot.contents[0] = rootElem;
    newRoot.child[0] = lAddr;
    newRoot.child[1] = rAddr;
    for(int i=2; i<ORDER; i++)
        newRoot.child[i] = -1; // set rest of children to -1 to prevent segfault
    
    // write out newRoot to file. write out dummy header information at beginning
    treeFile.seekp(0, ios::end);
    int newRootAddr = treeFile.tellp();
    treeFile.write((char*)&newRoot, sizeof(BTNode));
    
    BTNode dummy;
    dummy.child[0] = newRootAddr; // set new root address for header
    treeFile.seekp(0, ios::beg);
    treeFile.write((char*)&dummy, sizeof(BTNode));

    write+=2;

    rootAddr = newRootAddr;
    root = newRoot;
}

void BTree::placeNode(keyType key, int pAddr, int lAddr, int rAddr) {
    // 2 cases:
    //   1. the parent node is not full. push in key and sort.
    //   2. the parent node is full. ricochet split occurs.

    BTNode parent = getNode(pAddr);
    if(parent.currSize<ORDER-1) {
        // sorts contents of the BTNode, specifically for the purpose of 
        // organizing data for not only the contents but also the children
        // to allow proper splitting
        int shift = 0;
        // grab shift value that determines where the key will be inserted
        while(key > parent.contents[shift] && shift < parent.currSize)
            shift++;
        int end = parent.currSize-1;
        while(end>=shift) { // shift contents over 1 cell each
            parent.contents[end+1] = parent.contents[end];
            parent.child[end+2] = parent.child[end+1];
            end--;
        }
        parent.contents[shift] = key;
        parent.child[shift+1] = rAddr;
        parent.currSize++;

        treeFile.seekp(pAddr);
        treeFile.write((char*)&parent, sizeof(BTNode));
        write++;

        // reflect changes on root
        if(pAddr == rootAddr) {
            root = parent;
        }
    }

    else {
        // recursively split nodes until case 1 is satisfied or new root is generated
        // key cannot be added because this node is full. instead, perform a sort on the
        // parent and find the new mid value of the parent. Then, recursively call this function.

        int newRecordAddr;
        splitNode(key, pAddr, newRecordAddr, rAddr);
    }
}

// credit to Dr. Digh for this method
void BTree::printNode(int recAddr) {
    // print the node provided by address
    treeFile.seekg(recAddr, ios::beg);
    BTNode dummy;
    treeFile.read((char *) &dummy, sizeof(BTNode));
    read++;
    cout<<"    *** node of size "<<dummy.currSize<<" ***     "<<endl;
    for(int i=0; i<dummy.currSize; i++)
        cout<<dummy.contents[i]<<endl;
}
///////////////////////
