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
    int fingerTable[7];
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
    int getDistance(int ID1, int ID2);

    Message* lookup(Message* msg);
    KeyValue* insert(KeyValue* msg);

};


class Chord {
public:

    // meta data
    // only used when a new node wants to know an existing node in Chord Network
    int m;
    int size;

    // Node nodeRouter[SIZE];
    vector<Node*> nodeRouter;
    // vector<int> nodeList;
    int nodeNum = 0;

    // the map from node ID to vector index
    unordered_map<int, int> indexes;

    int value; // used to get the final result

    Chord(int _M, int _SIZE);

    void createNewNode(int ID);
    void createFirstNode();


    Node* routing(int ID);

    int findSuccessor(Message* msg);


    void join(int ID);
    void joinNodes(int num);

    // get node ID's successor's predecessor
    int getSuccPred(int ID);
    void notifySucc(int ID);
    void updateSuccPred(int ID);
    void updateFingerTable(int ID);

    void showNodeList();

    void showStorage(int ID);
    void showNodesStorage();
    void showFingerTable(int ID);
    void showNodesFingerTable();

    void showPred(int ID);
};

#endif
