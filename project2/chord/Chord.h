#ifndef _CHORD_H
#define _CHORD_H

#include <math.h>
#include <vector>
#include <unordered_map>
#include "Message.h"

using namespace std;

// Forward Declaration to solve cyclic dependency
class Node {
public:
    int m;
    int size;

    // ID in the Chord Ring
    int ID;

    // finger table
    int* fingerTable;
    int predecessor = -1;

    // key-value storage
    unordered_map<int, int> storage;

    // store the recent query value
    bool exist;
    int value;

    Node(int _ID, int _M, int _SIZE);
    // argument: key,
    // return: next hop, possible stored server ID
    bool ifExist(int key);
    int getValue(int key);

    // compute the distance in Chord Ring
    int getDistance(int ID1, int ID2);

    // send lookup message to another node
    Message* lookup(Message* msg);
    // send insert message to another node
    KeyValue* insert(KeyValue* msg);
};


class Chord {
public:
    // meta data
    // only used when a new node wants to know an existing node in Chord Network
    int m;
    int size;

    vector<Node*> nodeRouter;
    int nodeNum = 0;

    // the map from node ID to vector index
    unordered_map<int, int> indexes;


    // Function
    Chord(int _M, int _SIZE);

    void createNewNode(int ID);
    void createFirstNode();

    Node* routing(int ID);

    int findSuccessor(Message* msg);

    // get node ID's successor's predecessor
    // update Notes' finger table/ predecessor/ successor
    int getSuccPred(int ID);

    // stabilization function
    void notifySucc(int ID);
    void updateSuccPred(int ID);
    void updateBasicFingerTable(int ID);
    void updateFingerTable(int ID);

    // ****** API provided for user ******//
    void join(int ID);           // node ID join Chord
    void joinNodes(int num);     // generate multiple(num) nodes to join Chord

    // for user's check
    void showStorage(int ID);                  // show key-value storage of node ID
    void showNodesStorage();                   // show key-value storage of all nodes
    void showFingerTable(int ID);              // show finger table of node ID
    void showNodesFingerTable();               // show finger table of all nodes
    void showNodeList();
    void showPred(int ID);                     // show predcessor of node ID

    // insert and lookup API
    void insertValue(int key, int value);
    void insertMultiValues(int length, int* keys, int* values);
    void findKey(int key);
    void findMultiKeys(int length, int* keys);
};

#endif
