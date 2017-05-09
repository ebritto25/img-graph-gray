#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <igraph.h>
#include <iostream>
#include "vectorgraph.hpp"


using namespace cv;
using namespace std;

void mostrarimg(Mat img)
{
    namedWindow("Imagem");
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

int main(int argc, char *argv[])
{
    igraph_t graph;

    VectorGraph vec;

    igraph_real_t val = 10;


    // testando o objeto
    vec.insert(0,val);


    std::cout << vec[0] << '\n';
    // =======

    char s;
    igraph_vector_t edges,weights;
    if(argc != 2)
        exit(1);

    Mat image = imread(argv[1]);
    cvtColor(image,image,COLOR_RGB2GRAY);

    //mostrarimg(image);
    cout << image.rows << "*" << image.cols << "=" << image.cols*image.rows << endl;

    graph = createGraph(image);

    igraph_vector_init(&edges,(2*(2*image.cols*image.rows+image.cols-image.rows)));
    igraph_vector_init(&weights,(2*image.cols*image.rows+image.cols-image.rows));

    EWVector(image,&edges,&weights);

    igraph_vector_destroy(&edges);
    igraph_vector_destroy(&weights);

    igraph_destroy(&graph);



    return 0;
}
