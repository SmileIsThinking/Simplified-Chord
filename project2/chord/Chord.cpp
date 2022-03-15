#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "Chord.h"
#include "Node.h"
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
    int index = indexes[ID];
    Node* node = nodeRouter[index];
    return node;
}

int Chord::findSuccessor(Message* msg) {
    Node* node;
    while(msg->flag != 1) {
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
    node = routing(succ);
    return node->predecessor;
}

void Chord::notifySucc(int ID) {
    Node* node = routing(ID);
    int succ_pred = getSuccPred(ID);
    if(succ_pred == -1) {
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
        }else {
            // succ_pred | node (new node) | successor
            // update successor's predecessor
            updateSuccPred(ID);
        }
    }
    return;
}

void Chord::updateSuccPred(int ID) {
    Node* node = routing(ID);
    int succ = node->fingerTable[0];
    node = routing(succ);
    node->predecessor = ID;
    // for(int i = 0; i < node->m; i++) {
    //     if(node->fingerTable[i] == node->ID) {
    //         int dist1 = node->getDistance(node->ID, node->ID + pow(2, i));
    //         int dist2 = node->getDistance(node->predecessor, node->ID + pow(2, i));
    //         if(dist1 < dist2){
    //             node->fingerTable[i] = node->predecessor;
    //         }else {
    //             break;
    //         }
    //     }
    // }
    // if the successor is its self, means it needs to update
    if(node->fingerTable[0] == node->ID) {
        node->fingerTable[0] = node->predecessor;
    }
    return;
}

// fix finger table:
// if distance <= successor, this finger table value is successor
// if distance > successor, send query to successor.

void Chord::updateFingerTable(int ID) {
    Node* node = routing(ID);
    for(int i = 1; i < node->m; i++) {
        int dist1 = pow(2, i);
        int dist2 = node->fingerTable[0] - node->ID;
        if(dist1 <= dist2) {
            node->fingerTable[i] = node->fingerTable[0];
        }else {
            int key = node->ID + pow(2, i);
            int nextHop = node->fingerTable[0];
            Message* msg = new Message(key, nextHop, 0);
            node->fingerTable[i] = findSuccessor(msg);
        }
    }
    return;
}


void Chord::join(int ID) {
    Node* node = new Node(ID, this->m, this->size);
    this->nodeNum++;
    this->nodeRouter.emplace_back(node);

    // join Chord Network by public node 0
    int nextHop = 0;
    int key = ID+1;
    Message* msg = new Message(key, nextHop, 0);
    node->fingerTable[0] = findSuccessor(msg);

    for(int i = 0; i < nodeNum; i++) {
        notifySucc(node->ID);
        node = routing(node->fingerTable[0]);
    }
    for(int i = 0; i < nodeNum; i++) {
        node = routing(node->fingerTable[0]);
        updateFingerTable(node->ID);
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
        join(newID);
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
    for(int i = 0; i < M; i++) {
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