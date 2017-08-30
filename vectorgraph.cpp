
VectorGraph::VectorGraph(int _size)
{
    igraph_vector_init(&vec,_size);
}

VectorGraph::~VectorGraph()
{
    igraph_vector_destroy(&vec);
}

igraph_vector_t* VectorGraph::getVec()
{
    return &vec;
}

void VectorGraph::pop(int value)
{
    igraph_vector_remove(&vec, value);
}

void VectorGraph::sort()
{
    igraph_vector_sort(&vec);
}

void VectorGraph::insert(int index,const igraph_real_t val)
{
    if(index > size())
    {
        igraph_vector_resize(&vec, size() * 2);
    }

    VECTOR(vec)[index] = val;
}


void  VectorGraph::send_toIgraphVector(igraph_vector_t* vector)
{
     igraph_vector_copy(vector,&vec);
}

int VectorGraph::size()
{
    return igraph_vector_size(&vec);
}

