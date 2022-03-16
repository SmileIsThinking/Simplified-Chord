#include "Chord.h"
#include <random>
#include <iostream>
// #include <random>

// Test Chord System in m = 7 and size = 128 (which could be changed)
#define CHORD_M 7

// The number of Node in Chord System
#define NODE_NUM 5

// The number of tested key value pairs
#define KEY_LENGTH 10

void generatePairs(int length, int* keys, int* values, int size);

int main() {
    int m = CHORD_M;
    int size = pow(2, CHORD_M);
    Chord* chord = new Chord(m, size);

    // int nodeNum = 5;
    chord->joinNodes(NODE_NUM);


    chord->showNodeList();
    chord->showNodesFingerTable();

    // chord->createFirstNode();
    // chord->join(92);
    // chord->join(68);
    // chord->join(15);
    // chord->join(35);
    // chord->join(76);


    const int length = KEY_LENGTH;
    int* keys = new int[length];
    int* values = new int[length];

    generatePairs(length, keys, values, size);

    chord->insertMultiValues(length, keys, values);
    chord->showNodesStorage();

    chord->findMultiKeys(length, keys);

    // chord->showNodesFingerTable();
    // std::random_device dev;
    // std::mt19937 rng(dev());
    // std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6); // distribution in range [1, 6]

    // for(int i = 0; i < 15; i++){
    //     std::cout << dist6(rng) << std::endl;
    // }

    // chord.generateNodes();
    // chord.showNodeList();

    // int* testKey = generateKeys(int num);
    // unordered_map<int, int>* testKeyValue = generatePairs(int num);
    // showTestKeyValue(testKeyValue);

    // insertToChord(&chord, testKeyValue);
    // chord.showNodeStorage();

    // int* rspValue;
    // int* rspNode;


    // keyQuery(testKey, rspValue, rspNode);

    // showFinalResults(testKey, rspValue, rspNode);
    // compareToTruth();

    // unique_ptr<Message> msg(new Message());
    // while(msg->flag != 2) {

    // }
    // if(msg->flag == 2){

    // }
    delete[] keys;
    delete[] values;

    return 0;
}

void generatePairs(int length, int* keys, int* values, int size)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    // distribution in range [1, size-1]
    std::uniform_int_distribution<std::mt19937::result_type> keyGen(0,size-1);
    std::uniform_int_distribution<std::mt19937::result_type> valueGen(0, 1000);

    for(int i = 0; i < length; i++){
        keys[i] = keyGen(rng);
        values[i] = valueGen(rng);
    }

}

// void showFinalResults(int* testKey, int* rspValue, int* rspNode)
// {

// }
