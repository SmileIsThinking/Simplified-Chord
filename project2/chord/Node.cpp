
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>

#include "Message.h"
#include "Chord.h"

using namespace std;

Node::Node(int _ID, int _M, int _SIZE): ID(_ID), m(_M), size(_SIZE)
{
    this->fingerTable  = new int[m];
    for(int i = 0; i < m; i++){
        this->fingerTable[i] = this->ID;
    }
}

bool Node::ifExist(int key) {
    if(storage.find(key) != storage.end()) {
        return true;
    }else {
        return false;
    }
}

int Node::getValue(int key) {
    return storage[key];
}


// ID1: this->ID
// ID2: key/next node
int Node::getDistance(int ID1, int ID2) {
    int r1 = ID1 % (this->size);
    int r2 = ID2 % (this->size);
    if(r1 <= r2) {
        return (r2 - r1);
    }else {
        return (size - r1 + r2);
    }
}

Message* Node::lookup(Message* msg) {
    if(msg->flag == 1) {
        // cout << "This Hop: " << msg->nextHop << endl;
        // this_thread::sleep_for(std::chrono::milliseconds(100));
        msg->flag = 2;
        msg->nextHop = this->ID;
        this->exist = ifExist(msg->key);
        if(this->exist == true) {
            this->value = getValue(msg->key);
        }
        return msg;
    }
    else if(msg->flag == 0) {
        // cout << "This Hop: " << msg->nextHop << endl;
        // this_thread::sleep_for(std::chrono::milliseconds(100));
        int i = 0;
        for(i = 0; i < m; i++){
            int dist1 = getDistance(this->ID, fingerTable[i]);
            int dist2 = getDistance(this->ID, msg->key);
            if(dist1 <= dist2) {
                msg->nextHop = fingerTable[i];
            }else {
                break;
            }
        }
        if(i == 0) {
            // does not find the largest node <= key
            // send to successor and do the query there
            msg->nextHop = fingerTable[0];
            msg->flag = 1;
        } else{
            // find the largest node <= key
            msg->flag = 0;
            if(msg->nextHop == this->ID) {
                msg->flag = 1;
            }

        }
    }

    return msg;
}

KeyValue* Node::insert(KeyValue* msg) {

    if(msg->flag == 1) {
        msg->flag = 2;
        msg->nextHop = this->ID;

        storage[msg->key] = msg->value;
        return msg;
    }
    else if(msg->flag == 0) {
        int i = 0;
        for(i = 0; i < m; i++){
        int dist1 = getDistance(this->ID, fingerTable[i]);
        int dist2 = getDistance(this->ID, msg->key);
            if(dist1 <= dist2) {
                msg->nextHop = fingerTable[i];
            }else {
                break;
            }
        }
        if(i == 0) {
            // does not find the largest node <= key
            // send to successor and store the value there
            msg->nextHop = fingerTable[0];
            msg->flag = 1;
        } else{
            // find the largest node <= key
            msg->flag = 0;
        }
    }

    return msg;
}






