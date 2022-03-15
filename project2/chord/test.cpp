#include "Chord.h"
#include <random>
#include <iostream>
// #include <random>

int main() {
    int m = 7;
    int size = pow(2, m);
    Chord* chord = new Chord(m, size);
    // chord->joinNodes(1);
    // chord->join(50);


    int nodeNum = 6;
    chord->joinNodes(nodeNum);
    // chord->createFirstNode();
    // chord->join(80);
    // chord->join(102);
    // chord->join(19);

    chord->showNodeList();
    chord->showNodesFingerTable();
    chord->showNodesStorage();

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
    return 0;
}

// int* generateKeys(int num)
// {
//     return nullptr;
// }

// unordered_map<int, int>* generatePairs(int num, int* testKey)
// {

// }

// void showFinalResults(int* testKey, int* rspValue, int* rspNode)
// {

// }
