#ifndef VECTORGRAH_H
#define VECTORGRAH_H

#include <igraph.h>

class VectorGraph
{
public:
    VectorGraph(int _size = 256);

    igraph_real_t operator[] (int index) {  return (VECTOR(vec)[index]); }

    ~VectorGraph();

    int size();


    void insert(int,igraph_real_t);

private:
    igraph_vector_t vec;
    int Size;
};




#include "vectorgraph.cpp"


#endif // VECTORGRAH_H
