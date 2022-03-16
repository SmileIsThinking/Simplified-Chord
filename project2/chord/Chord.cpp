#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "Chord.h"
// #include "Node.h"
// #include <crypto++/sha.h>

using namespace std;

Chord::Chord(int _M, int _SIZE):
    m(_M), size(_SIZE) {}


void Chord::createNewNode(int ID) {
    Node* node = new Node(ID, this->m, this->size);
    this->nodeNum++;
    this->nodeRouter.emplace_back(node);
    indexes[node->ID] = nodeRouter.size()-1;
}

void Chord::createFirstNode() {
    this->createNewNode(0);
}

Node* Chord::routing(int ID) {
    // cout << "routing" << endl;
    int index = indexes[ID];
    Node* node = nodeRouter[index];
    return node;
}

int Chord::findSuccessor(Message* msg) {
    Node* node;
    while(msg->flag != 1) {
        // cout << "flag: " << msg->flag << endl;
        node = routing(msg->nextHop);
        msg = node->lookup(msg);
    }
    return msg->nextHop;
}

// // get node ID's successor's predecessor
// int Chord::getSuccPred(int ID) {
//     Node node* = routing(ID);
//     int succ =
//     Node node* =
// }


int Chord::getSuccPred(int ID) {
    Node* node = routing(ID);
    int succ = node->fingerTable[0];
    // cout << "Succ: " << succ << endl;
    node = routing(succ);
    return node->predecessor;
}

void Chord::notifySucc(int ID) {
    Node* node = routing(ID);
    // cout << "notify from: " << node->ID << endl;
    int succ_pred = getSuccPred(ID);
    // cout << "succ_pred: " << succ_pred << endl;
    if(succ_pred == -1) {
        // cout << "update Succ Pred: " << ID << endl;
        updateSuccPred(ID);
        return;
    }
    if(succ_pred != ID) {
        int dist1 = node->getDistance(ID, succ_pred);
        int dist2 = node->getDistance(ID, node->fingerTable[0]);
        if(dist1 < dist2) {
            // node | succ_pred (new node) | successor
            // update my successor
            node->fingerTable[0] = succ_pred;
            notifySucc(node->ID);
        }else {
            // succ_pred | node (new node) | successor
            // update successor's predecessor
            updateSuccPred(ID);
        }
    }
    // showPred(50);
    return;
}

void Chord::showPred(int ID) {
    Node* node = routing(ID);
    cout << "--------------" << endl;
    cout << "show Node Pred: " << node->ID << endl;
    cout << node->predecessor << endl;
    cout << "--------------" << endl;
    return;
}

void Chord::updateSuccPred(int ID) {
    Node* node = routing(ID);
    int succ = node->fingerTable[0];
    node = routing(succ);
    // cout << "update node: " << node->ID << endl;
    node->predecessor = ID;

    // if the successor is its self, means it needs to update
    if(node->fingerTable[0] == node->ID) {
        node->fingerTable[0] = node->predecessor;
    }
    return;
}

// fix finger table:
// if distance <= successor, this finger table value is successor
// if distance > successor, send query to successor.

void Chord::updateBasicFingerTable(int ID) {
    Node* node = routing(ID);
    for(int i = 1; i < node->m; i++) {
        // cout << "m: " << node->m << endl;
        int dist1 = pow(2, i);
        int dist2 = node->getDistance(node->ID, node->fingerTable[0]);
        if(dist1 <= dist2) {
            node->fingerTable[i] = node->fingerTable[0];
        }
    }
    return;
}

void Chord::updateFingerTable(int ID) {
    Node* node = routing(ID);
    for(int i = 1; i < node->m; i++) {
        // cout << "m: " << node->m << endl;
        int dist1 = pow(2, i);
        int dist2 = node->getDistance(node->ID, node->fingerTable[0]);
        if(dist1 <= dist2) {
            node->fingerTable[i] = node->fingerTable[0];
        }else {
            int key = (int)(node->ID + pow(2, i)) % (node->size);
            // cout << "i: " << i << endl;
            int nextHop = node->fingerTable[i-1];
            Message* msg = new Message(key, nextHop, 0);
            node->fingerTable[i] = findSuccessor(msg);
        }
    }
    return;
}


void Chord::join(int ID) {
    createNewNode(ID);
    Node* node = routing(ID);

    // join Chord Network by public node 0
    int nextHop = 0;
    int key = ID+1;
    Message* msg = new Message(key, nextHop, 0);
    // cout << "message" << endl;
    node->fingerTable[0] = findSuccessor(msg);

    for(int i = 0; i < nodeNum; i++) {
        // cout << "Your turn to notify: " << node->ID << endl;
        notifySucc(node->ID);
        node = routing(node->fingerTable[0]);
    }

    for(int i = 0; i < nodeNum; i++) {
        node = routing(node->fingerTable[0]);
        updateBasicFingerTable(node->ID);
        // cout << "You have modify the basic fingertable: " << node->ID << endl;
        // showFingerTable(node->ID);
    }

    for(int i = 0; i < nodeNum; i++) {
        node = routing(node->fingerTable[0]);
        updateFingerTable(node->ID);
        // cout << "You have modify the fingertable: " << node->ID << endl;
        // showFingerTable(node->ID);
    }
}

void Chord::joinNodes(int num) {

    createFirstNode();

    random_device dev;
    mt19937 rng(dev());

    // distribution in range [1, size-1]
    uniform_int_distribution<mt19937::result_type> dist(1, size-1);
    for(int i = 0; i < num-1; i++) {
        int newID = dist(rng);
        if(indexes.find(newID) == indexes.end()){
            cout << "!!!!!!!" << endl;
            cout << "New Node: " << newID << endl;
            join(newID);
        }
    }
}

void Chord::insertValue(int key, int value) {
    Message* msg = new Message(key, 0, 0);
    int target = findSuccessor(msg);
    delete msg;
    KeyValue* msg2 = new KeyValue(key, value, target, 1);
    Node* node = routing(target);
    node->insert(msg2);
    delete msg2;
    return;
}

void Chord::insertMultiValues(int length, int* keys, int* values) {
    for(int i = 0; i < length; i++) {
        // cout << "key: " << keys[i] << endl;
        // cout << "value: " << values[i] << endl;
        this->insertValue(keys[i], values[i]);
    }
}

void Chord::findKey(int key) {
    int loc = rand() % this->nodeNum;
    Node* node = nodeRouter[loc];

    Message* msg = new Message(key, node->ID, 0);
    while(msg->flag != 2) {
        msg = node->lookup(msg);
        node = routing(msg->nextHop);
    }

    cout << "--------------------------" << endl;
    if(node->exist == true){
        cout << "| Have found the key: " << key << endl;
        cout << "| Value: " << node->value << endl;
        cout << "| Found in Node: " << node->ID << endl;
    } else{
        cout << "!!!! The key " << key << " does not exist!!!!" << endl;
    }
    cout << "--------------------------" << endl;
}

void Chord::findMultiKeys(int length, int* keys) {
    for(int i = 0; i < length; i++) {
        findKey(keys[i]);
    }
}

void Chord::showNodeList() {
    cout << "--------------------------" << endl;
    cout << "Node Num: " << nodeRouter.size() << endl;
    cout << "Node ID List" << endl;
    for(auto& nodePtr: this->nodeRouter) {
        cout << nodePtr->ID << endl;
    }
    cout << "--------------------------" << endl;
    return;
}

void Chord::showStorage(int ID) {
    Node* node = routing(ID);
    cout << "--------------------------" << endl;
    cout << " Key Value Storage of node " << ID << endl;
    if((node->storage).empty() == true) {
        cout << "The Storage is EMPTY!!!!" << endl;
    }else{
        for(auto& p: node->storage) {
            cout << "Key: " << p.first << " | " << "Value: " << p.second << endl;
        }
    }
    cout << "--------------------------" << endl;
}

void Chord::showNodesStorage() {
    Node* node = routing(0);
    for(int i = 0; i < nodeNum; i++) {
        showStorage(node->ID);
        node = routing(node->fingerTable[0]);
    }
}

void Chord::showFingerTable(int ID) {
    Node* node = routing(ID);
    cout << "--------------------------" << endl;
    cout << " Finger Table of node " << ID << endl;
    for(int i = 0; i < this->m; i++) {
        cout << i << ": " << node->fingerTable[i] << endl;
    }
    cout << "--------------------------" << endl;
}

void Chord::showNodesFingerTable() {
    Node* node = routing(0);
    for(int i = 0; i < nodeNum; i++) {
        showFingerTable(node->ID);
        node = routing(node->fingerTable[0]);
    }
}