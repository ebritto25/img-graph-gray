#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <igraph.h>
#include <iostream>

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
    int cont = 0,pixel = 0,wcont = 0;


    for(int i = 0;i < img.rows;i++)
    {
        for(int j = 0;j < img.cols;j++,pixel++)
        {
            if((i < img.rows - 1) && (j < img.cols - 1))//PIXEL NÃO É BORDA, LIGA A DIREITA E ABAIXO
            {
                //LIGA COM O VERTICE DA DIREITA E CALCULA O PESO
                VECTOR(*edges)[cont] = pixel;
                VECTOR(*edges)[cont+1] = pixel+1;
                VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i,j + 1)));
                //LIGA COM O VERTICE DE BAIXO E CALCULA O PESO
                VECTOR(*edges)[cont+2] = pixel;
                VECTOR(*edges)[cont+3] = pixel+(img.cols);
                igraph_vector_set(weight,wcont++,abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j))));
                //VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j)));
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
                    VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i,j + 1)));
                    cont += 2;
                }
                if(j == (img.cols - 1))//PIXEL NA BORDA DIREITA, LIGA SÓ ABAIXO
                {
                    VECTOR(*edges)[cont] = pixel;
                    VECTOR(*edges)[cont+1] = pixel+(img.cols);
                    VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j)));
                    cont += 2;
                }
            }
        }
    }
    return 1;
}

void print_vector(igraph_vector_t *v) {
  long int i, l=igraph_vector_size(v);
  for (i=0; i<l; i++) {
    cout << (long int) VECTOR(*v)[i] << "->";
  }
  printf("\n");
}


int main(int argc, char *argv[])
{
    igraph_t graph;
    igraph_vector_t edges,weights;
    igraph_vector_ptr_t vPath,ePath;
    igraph_vector_long_t pred,inbound;
    igraph_vs_t to;
    int from;

    if(argc != 2)
        exit(1);

    Mat image = imread(argv[1]);
    cvtColor(image,image,COLOR_RGB2GRAY);

    //CRIAÇÃO DO GRAFO
    graph = createGraph(image);

    //INICIALIZAÇÃO DE VETORES
    igraph_vector_init(&edges,(2*(2*image.cols*image.rows-image.cols-image.rows)));
    igraph_vector_init(&weights,(2*image.cols*image.rows-image.cols-image.rows));
    igraph_vector_ptr_init(&vPath,1);
    igraph_vector_ptr_init(&ePath,1);
    igraph_vector_long_init(&pred,0);
    igraph_vector_long_init(&inbound,0);

    VECTOR(vPath)[0] = calloc(1,sizeof(igraph_vector_t));
    VECTOR(ePath)[0] = calloc(1,sizeof(igraph_vector_t));

    igraph_vector_init((igraph_vector_t*)VECTOR(vPath)[0], 0);
    igraph_vector_init((igraph_vector_t*)VECTOR(ePath)[0], 0);

    from = 0;//ID DO PIXEL DE PARTIDA
    igraph_vs_1(&to,24);//ID DO PIXEL DESTINO

    //DETERMINAÇÃO DAS ARESTAS E ADIÇÃO NO GRAFO
    EWVector(image,&edges,&weights);
    igraph_add_edges(&graph,&edges,0);


    //CALCULA E IMPRIME MENOR CAMINHO
    igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from,to,&weights,IGRAPH_ALL,&pred,&inbound);
    print_vector((igraph_vector_t*)VECTOR(vPath)[0]);

    cout << '\n';

    //DESTRUIÇÃO DOS ELEMENTOS
    igraph_vector_destroy((igraph_vector_t*)VECTOR(vPath)[0]);
    igraph_vector_destroy((igraph_vector_t*)VECTOR(ePath)[0]);
    igraph_vector_ptr_destroy(&vPath);
    igraph_vector_ptr_destroy(&ePath);
    igraph_vector_destroy(&edges);
    igraph_vector_destroy(&weights);
    igraph_destroy(&graph);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*cvtColor(image,image,COLOR_RGB2GRAY);

    //mostrarimg(image);
    cout << image.rows << "*" << image.cols << "=" << image.cols*image.rows << endl;


*/

    return 0;
}
