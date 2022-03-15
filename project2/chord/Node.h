#ifndef _NODE_H_
#define _NODE_H_

#include <unordered_map>
#include "Chord.h"
#include "Message.h"

using namespace std;

const int M = 7;
const int SIZE = pow(2, M);

// class Node {
// public:
//     int m;
//     int size;

//     // ID in the Chord Ring
//     int ID;

//     static int M = 7;
//     // finger table
//     int fingerTable[M];
//     int predecessor = -1;

//     // key-value storage
//     std::unordered_map<int, int> storage;


//     // store the recent query value
//     bool exist;
//     int value;

//     Node(int _ID, int _M, int _SIZE);
//     // argument: key,
//     // return: next hop, possible stored server ID
//     bool ifExist(int key);
//     int getValue(int key);
//     int getDistance(int ID1, int ID2);

//     Message* lookup(Message* msg);
//     KeyValue* insert(KeyValue* msg);

//     // get node ID's successor's predecessor
//     int getSuccPred();
//     void notifySucc();
//     void updateSuccPred();
//     void updateFingerTable();
// };


#endif