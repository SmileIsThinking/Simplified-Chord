
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Node.h"
#include "Message.h"
#include "Chord.h"

using namespace std;

Node::Node(int _ID, int _M, int _SIZE): ID(_ID), m(_M), size(_SIZE)
{
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
    if(ID1 <= ID2) {
        return (ID2 - ID1);
    }else {
        return (size - ID1 + ID2);
    }
}

Message* Node::lookup(Message* msg) {
    if(msg->flag == 1) {
        msg->flag = 2;
        msg->nextHop = this->ID;
        this->exist = ifExist(msg->key);
        if(this->exist == true) {
            this->value = getValue(msg->key);
        }
        return msg;
    }
    else if(msg->flag == 0) {
        int i = 0;
        int dist1 = getDistance(this->ID, fingerTable[i]);
        int dist2 = getDistance(this->ID, msg->key);
        for(i = 0; i < m; i++){
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
        int dist1 = getDistance(this->ID, fingerTable[i]);
        int dist2 = getDistance(this->ID, msg->key);
        for(i = 0; i < m; i++){
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






