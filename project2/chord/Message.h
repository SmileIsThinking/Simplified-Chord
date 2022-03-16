#ifndef _MESSAGE_H
#define _MESSAGE_H

// flag == 0: query to the nextHop
// flag == 1: query to lookup immediately when the server receives
// flag == 2: feedback message to the client (potentially used, design redundancy)
class Message {
public:
    int key;
    int nextHop;
    int flag;

    Message(int _key, int _nextHop, int _flag):
    key(_key), nextHop(_nextHop), flag(_flag){}
};

class KeyValue {
public:
    int key;
    int value;
    int nextHop;
    int flag;

    KeyValue(int _key, int _value, int _nextHop, int _flag):
    key(_key), value(_value), nextHop(_nextHop), flag(_flag){}
};

#endif