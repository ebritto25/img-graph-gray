
VectorGraph::VectorGraph(int _size) : Size(_size)
{
    igraph_vector_init(&vec,_size);
}

VectorGraph::~VectorGraph()
{
    igraph_vector_destroy(&vec);
}


void VectorGraph::insert(int index, igraph_real_t val)
{
    VECTOR(vec)[index] = val;
}

int VectorGraph::size()
{
    return Size;
}


