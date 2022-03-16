# Simplified Chord System Simulation

## Introduction

The project is conducted for the UW-Madison CS740 AS2, re-implementing Chord scalable lookup system based on this paper https://pdos.csail.mit.edu/papers/ton:chord/paper-ton.pdf.

**github link**: https://github.com/SmileIsThinking/cs740/tree/main/project2


## Background
"Chord" is a protocol for peer-to-peer systems to determine the storage location of key-value pairs and the lookup path, with several proven advantages including high performance, high scalability, uniform load balancing, and failure tolerance. Chord algorithm uses consistent hashing on node’s (peer’s) address and introduces finger table as routing table to each node. Each node only needs to store a small number of entries, and the cost of searching is reduced to O(log N). And the stabilization algorithm is used to update finger tables and successor pointers periodically to handle situations like nodes joining and leaving, which provides a reachability guarantee. Furthermore, Chord maintains a successor list to handle failure in case of immediate successor does not respond.


## What does my simple Chord support?
In this project, I reproduce a simple program to simulate the distributed Chord System. Instead of using RPC for communicaition, this program has only one process: Chord, which has different server nodes and these nodes are "independent". Nodes communicate to each other using "message" (defined in Message.h) and Chord plays a role as a router, directly routing message to the next hop. As for stabilization, this program simulates a single-thread, sequential stabilization algorithm. The stabilization algorithm contains two parts: pred/succ update and full finger table update. Stabilization functions are called when a new node joins Chord Network (Stabilization does not work periodly).


### My Chord supports
* Create Chord System (create the first Chord Node)
* Node join (one at a time)
* Sequential Stabilization Algorithm (update when a new node joins)
  - Predecessor Update
  - Successor Update
  - Finger Table Update
* Direct Routing
* Key-Value Insert
* Key-Value Lookup (Explicitly show whether it is existed)


### Does not support
* Node Delete
* Node and Link Failure
* Key-Value Migration
* Distributed Stabilization Algorithm
* ...

## Usage
* Prerequisite: g++ 7.5.0 (Every C++ compiler that supports C++11 should work.)
* Build:
```
source ./chord/build.sh
```
* Test:
```
./chord/test
```


## Design Overview
* **Message.h** - class Message is used to as lookup message between nodes. class KeyValue is used as insert message to nodes.
* **Node.cpp** - lookup and insert functions are implemented here as the communication between nodes.
* **Chord.cpp** - Play a role of "Router". Routing message to another node according to "nextHop" in message. Provide API for users.


## API provided
### Chord Operation
* Routing
  - Node* routing(int ID)

* Join
  - void join(int ID)
  - void joinNodes(int num)

* Insert
  - void insertValue(int key, int value)
  - void insertMultiValues(int length, int* keys, int* values)

* Lookup
  - void findKey(int key)
  - void findMultiKeys(int length, int* keys)

### Show Metadata
* Show Metadata for users
  - void showNodeList()
  - void showPred(int ID)
  - void showFingerTable(int ID)
  - void showNodesFingerTable()
  - void showStorage(int ID)
  - void showNodesStorage()

## User Test
* All tests should follow the following order (Please be aware that key-value migration is not supported):
  - join new nodes
  - insert key-value pairs
  - lookup and show results (show metadata of chord nodes)

* a simple test example (can be found in ./chord/test.cpp)
```
    int* keys = new int[length];
    int* values = new int[length];

    // create Chord System and multiple random nodes join
    Chord* chord = new Chord(m, size);
    chord->joinNodes(NODE_NUM);

    // generate multiple key-value pair for testing
    generatePairs(length, keys, values, size);

    // insert key-value pairs
    chord->insertMultiValues(length, keys, values);

    // show metadata of nodes
    chord->showNodeList();
    chord->showNodesFingerTable();
    chord->showNodesStorage();

    // find the stored node number of keys and show the result
    chord->findMultiKeys(length, keys);
```


## Author
* Shichun Yu - syu274@wisc.edu


