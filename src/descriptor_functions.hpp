#pragma once 


#include <opencv2/opencv.hpp>
#include <mutex>
#include <queue>
#include <iostream>
#include <functional>
#include <thread>
#include "image_base.cpp"
#include "boost_utils.hpp"
#include "vectorgraph.hpp"

#define DB(X) std::cout << #X << '=' << X << '\n'

using namespace cv;
using namespace std;

template<COLOR color>
void generate_edges_weights(const Mat &img,igraph_vector_t *edges,igraph_vector_t *weight,bool liga_8_camada = false);
template<COLOR color>
void destination_pixels(igraph_vs_t* to,const Mat& image);  // define os pixels de partida para o algoritmo dijkstra
template<COLOR color>
string atribute_generator(Mat &image, bool with_mst, bool do_dijkstra=true, bool liga_8_camada = false);

// gera uma string contendo os valores de média e desvio padrão do vetor(v)
string str_arff(igraph_vector_t *v);

// cria um grafo a partir da imagem passada
igraph_t createGraph(const Mat &imagem);

//gera um vetor resultante que contem as médias e os desvios padrões das distâncias
void avgVector(igraph_vector_t *edges,igraph_vector_t *weights, igraph_vector_t *res);

string produce_values(string_view arg,image_base& base,bool with_mst,bool do_dijkstra, bool liga_8_camada);

void extrai_valor(string_view folder,image_base& base,bool with_mst,bool do_dijkstra, bool liga_8_camada);

void thread_handler(image_base& base,bool do_dijkstra,bool with_mst,bool liga_8_camada);
