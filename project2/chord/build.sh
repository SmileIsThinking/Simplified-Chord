rm *.o
g++ -c Node.cpp
g++ -c Chord.cpp
g++ -o test test.cpp Node.o Chord.o --std=c++11