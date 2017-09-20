#ifndef VECTORGRAH_H
#define VECTORGRAH_H

#include <igraph.h>

class VectorGraph
{
public:
    VectorGraph(int _size = 0);

    ~VectorGraph();


    // OPERATORS
    igraph_real_t& operator[] (int index) {  return vec.stor_begin[index]; }
    igraph_vector_t* operator& () {  return &vec; }

    igraph_vector_t* igraph_vector();
    int size();
    void sort();
    void pop(int);
    void insert(int,const igraph_real_t);

private:
    igraph_vector_t vec;
};






#endif // VECTORGRAH_H
