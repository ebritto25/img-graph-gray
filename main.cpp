#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <igraph.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include "vectorgraph.hpp"


using namespace cv;
using namespace std;

void mostrarimg(Mat img)
{
    namedWindow("Imagem",WINDOW_AUTOSIZE);
    imshow("Imagem",img);
    waitKey(0);
}

igraph_t createGraph(Mat imagem)
{
    igraph_t graph;
    igraph_integer_t n = imagem.rows * imagem.cols;
    igraph_empty(&graph,n,IGRAPH_UNDIRECTED);


    cout << "Criado grafo "<< igraph_vcount(&graph) << " nós\n";
    return graph;
}


int EWVector(Mat img,igraph_vector_t *edges,igraph_vector_t *weight)
{
   // uchar *p;
    int cont = 0,pixel = 0,wcont = 0;
    for(int i = 0;i < img.rows;i++)
    {
        //p = img.ptr<uchar>(i);
        for(int j = 0;j < img.cols;j++,pixel++)
        {
            if(/*(i > 0) && (j > 0) &&*/(i < img.rows - 1) && (i < img.cols - 1))//PIXEL NÃO É BORDA, LIGA A DIREITA E ABAIXO
            {
                //LIGA COM O VERTICE DA DIREITA E CALCULA O PESO
                VECTOR(*edges)[cont] = pixel;
                VECTOR(*edges)[cont+1] = pixel+1;
                VECTOR(*weight)[wcont++] = abs((img.at<uchar>(i,j) - img.at<uchar>(i,j + 1)));
                //LIGA COM O VERTICE DE BAIXO E CALCULA O PESO
                VECTOR(*edges)[cont+2] = pixel;
                VECTOR(*edges)[cont+3] = pixel+(img.cols);
                VECTOR(*weight)[wcont++] = abs((img.at<uchar>(i,j) - img.at<uchar>(i+1,j)));
                //AVANÇA O CONTADOR DO VETOR 2 PARES A FRENTE
                cont += 4;
            }
            else
            {
                if((i == (img.rows - 1)) && (j == (img.cols - 1)))//ULTIMO PIXEL, NÃO FAZ NADA
                    continue;

                if(i == (img.rows - 1))//PIXEL NA BORDA INFERIOR, LIGA SÓ A DIREITA
                {
                    VECTOR(*edges)[cont] = pixel;
                    VECTOR(*edges)[cont+1] = pixel+1;
                    VECTOR(*weight)[wcont++] = abs((img.at<uchar>(i,j) - img.at<uchar>(i,j + 1)));
                    cont += 2;
                }
                if(j == (img.cols - 1))//PIXEL NA BORDA DIREITA, LIGA SÓ ABAIXO
                {
                    VECTOR(*edges)[cont] = pixel;
                    VECTOR(*edges)[cont+1] = pixel+(img.cols);
                    VECTOR(*weight)[wcont++] = abs((img.at<uchar>(i,j) - img.at<uchar>(i+1,j)));
                    cont += 2;
                }
            }
        }
    }
    return 1;
}


// calcula os pesos na imagem
bool image_weights(const Mat& image, VectorGraph& edges, VectorGraph& weights)
{
     int weight_iterator = 0, vertices_iterator = 0;

     for(int  i = 0; i < image.rows; i++)
     {
         for(int j = 0; j < image.cols; j++)
         {
             if(i == image.rows - 1 && j == image.cols -1) // final da imagem..
                   return true;
             if(!image.size)
                 return false;


             if( i < image.rows - 1 &&  j < image.cols - 1)
             {
                 // link com a direita
                    edges[vertices_iterator++] = j;
                    edges[vertices_iterator++] = j+1;
                    weights[weight_iterator++] = abs((image.at<uchar>(i,j) - image.at<uchar>(i,j+1)));


                 //  +++++

                 // link com o de baixo
                    edges[vertices_iterator++] = j;
                    edges[vertices_iterator++] = j + image.cols;

                    weights[weight_iterator++] = abs((image.at<uchar>(i,j) - image.at<uchar>(i+1,j)));
                 // +++++

             }
             else if( i == image.rows - 1) // borda inferior
             {
                 edges[vertices_iterator++] = j;
                 edges[vertices_iterator++] = j+1;
                 weights[weight_iterator++] =  abs((image.at<uchar>(i,j) - image.at<uchar>(i,j+1)));
             }
             else // borda direita
             {
                 edges[vertices_iterator++] = j;
                 edges[vertices_iterator++] = j + image.cols;
                 weights[weight_iterator++] = abs((image.at<uchar>(i,j) - image.at<uchar>(i+1,j)));
             }
         }

     }

     return false;
}




int main(int argc, char *argv[])
{
    //igraph_t graph;


    if(argc != 2)
        exit(1);

    Mat image = imread(argv[1],0);


    VectorGraph weights(2*image.cols*image.rows+image.cols-image.rows);
    VectorGraph edges(2*(2*image.cols*image.rows+image.cols-image.rows));



    image_weights(image,edges,weights);


    igraph_t graph;

    igraph_vector_t vec_edges;

    edges.send_toIgraphVector(&vec_edges);


    igraph_create(&graph,&vec_edges,0,0);

    igraph_matrix_t res;

    igraph_vector_t vec_weights;

    weights.send_toIgraphVector(&vec_weights);

    igraph_matrix_init(&res, 0, 0);

    igraph_shortest_paths_dijkstra(&graph,&res,igraph_vss_all(),igraph_vss_all(),&vec_weights,IGRAPH_ALL);



    if( image.empty() ) // Check for invalid input
    {
          cout <<  "Could not open or find the image" << '\n' ;
          return -1;
    }




    namedWindow("Imagem",WINDOW_AUTOSIZE);
    imshow("Imagem",image);
    waitKey(0);


    print_matrix(&res);

    return 0;
}

