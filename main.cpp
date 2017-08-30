#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <igraph.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <QElapsedTimer>
#include "vectorgraph.hpp"

using namespace cv;
using namespace std;

void mostrarimg(Mat img)
{
    namedWindow("Imagem");
    imshow("Imagem",img);
    waitKey(0);
}

igraph_t createGraph(Mat &imagem)
{
    igraph_t graph;

    igraph_integer_t n = imagem.rows * imagem.cols * imagem.channels();
    igraph_empty(&graph,n,IGRAPH_UNDIRECTED);


    cout << "Criado grafo "<< igraph_vcount(&graph) << " nós\n";
    cout << "Imagem: "<< imagem.cols << 'x' << imagem.rows << '\n';
    return graph;
}

int EWVector_gray(Mat & img,igraph_vector_t *edges,igraph_vector_t *weight)
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
                VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j)));
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

int EWVector(Mat &img,igraph_vector_t *edges,igraph_vector_t *weight)
{
    int cont = 0,pixel = 0,wcont = 0;
    Vec3b intensity1,intensity2;

    for(int camada = 0; camada < 3;camada++)
    {
        for(int i = 0;i < img.rows;i++)
        {
            for(int j = 0;j < img.cols;j++,pixel++)
            {
                if(camada < 2)
                {
                    intensity1 = img.at<Vec3b>(i,j);
                    VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows);
                    intensity2 = intensity1;
                    VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));
                }
                if((i < img.rows - 1) && (j < img.cols - 1))//PIXEL NÃO É BORDA, LIGA A DIREITA E ABAIXO
                {

                    //LIGA COM O VERTICE DA DIREITA E CALCULA O PESO
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+1;
                    intensity1 = img.at<Vec3b>(i,j);
                    intensity2 = img.at<Vec3b>(i,j + 1);
                    VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    //LIGA COM O VERTICE DE BAIXO E CALCULA O PESO
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+(img.cols);
                    intensity2 = img.at<Vec3b>(i+1,j);
                    VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    //AVANÇA O CONTADOR DO VETOR 2 PARES A FRENTE

                }
                else
                {
                    if((i == (img.rows - 1)) && (j == (img.cols - 1)))//ULTIMO PIXEL, NÃO FAZ NADA
                        continue;

                    if(i == (img.rows - 1))//PIXEL NA BORDA INFERIOR, LIGA SÓ A DIREITA
                    {
                        VECTOR(*edges)[cont++] = pixel;
                        VECTOR(*edges)[cont++] = pixel+1;
                        intensity1 = img.at<Vec3b>(i,j);
                        intensity2 = img.at<Vec3b>(i,j + 1);
                        VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    }
                    if(j == (img.cols - 1))//PIXEL NA BORDA DIREITA, LIGA SÓ ABAIXO
                    {
                        VECTOR(*edges)[cont++] = pixel;
                        VECTOR(*edges)[cont++] = pixel+(img.cols);
                        intensity1 = img.at<Vec3b>(i,j);
                        intensity2 = img.at<Vec3b>(i+1,j);
                        VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    }
                }
            }
        }
    }
    return 1;
}

string print_vector(igraph_vector_t *v) {
  long int i, l=igraph_vector_size(v);
  string res {""};
  for (i=0; i<l; i += 2) {
    res += to_string((double) VECTOR(*v)[i]) + "," + to_string((double) VECTOR(*v)[i+1])+",";
  }

  return res;
}

void avgVector(igraph_vector_t *edges,igraph_vector_t *weights, igraph_vector_t *res)
{
    igraph_real_t sum,des;
    int size = igraph_vector_size(edges);

    for(int i = 0;i < size;i++)
    {
        sum += (igraph_real_t)VECTOR(*weights)[(int)VECTOR(*edges)[i]];
    }

    sum /= size;

    igraph_vector_push_back(res,sum);

    for(int i = 0;i < size;i++)
    {
        des += pow(((igraph_real_t)VECTOR(*weights)[(int)VECTOR(*edges)[i]])-sum,2);
    }

    des /= size;
    des = sqrt(des);

    igraph_vector_push_back(res,des);
}



string atributeGenerator(string arg)
{
    igraph_t graph, mst;
    igraph_vector_t edges,weights,res;

    igraph_vector_ptr_t vPath,ePath;
    igraph_vector_long_t pred,inbound;
    igraph_vs_t to[16];

    Mat image = imread(arg);

    const int ultimo_pixel = (  (image.cols*image.rows)-1)+(image.cols*image.rows*2) ;
    const int primeiro_pixel_ultima = ((image.cols*image.rows)-image.cols)+(image.cols*image.rows*2);
    const int ultimo_pixel_1  = (image.cols*image.rows) -1;
    const int primeiro_pixel_ultima_1 = (image.cols*image.rows)-image.cols;
    const int meio_vertical_1 = (image.cols*(5/2));
    const int meio_horizontal_1 = ( (image.cols*image.rows)/2 ) + (image.cols-1);

    //INICIALIZAÇÃO DE VETORES
    VectorGraph vEdges((2*((image.channels()*(2*image.cols*image.rows-image.cols-image.rows))+(2*image.rows*image.cols))));
    VectorGraph vWeights((image.channels()*(2*image.cols*image.rows-image.cols-image.rows))+(2*image.rows*image.cols));
    igraph_vector_init(&res,0);
    igraph_vector_ptr_init(&vPath,1);
    igraph_vector_ptr_init(&ePath,1);
    igraph_vector_long_init(&pred,0);
    igraph_vector_long_init(&inbound,0);


    VECTOR(vPath)[0] = calloc(1,sizeof(igraph_vector_t));
    VECTOR(ePath)[0] = calloc(1,sizeof(igraph_vector_t));

    igraph_vector_init((igraph_vector_t*)VECTOR(vPath)[0], 0);
    igraph_vector_init((igraph_vector_t*)VECTOR(ePath)[0], 0);


    graph = createGraph(image);

    EWVector(image,vEdges.getVec(),vWeights.getVec());
    igraph_add_edges(&graph,vEdges.getVec(),0);

    //PIXELS DE PARTIDA
    int from[] = {0,(image.cols-1),(image.cols-1 + image.cols*image.rows * 2),(image.cols*image.rows*2),
                  0,(image.cols-1),image.cols/2,image.cols*(image.rows/2),
                    image.cols*image.rows,(image.cols-1)+(image.cols*image.rows),(image.cols/2)+(image.cols*image.rows),image.cols*(image.rows/2)+(image.cols*image.rows),
                    image.cols*image.rows*2,(image.cols-1)+(image.cols*image.rows*2),(image.cols/2)+(image.cols*image.rows*2),image.cols*(image.rows/2)+(image.cols*image.rows*2)};

    //PIXELS DE DESTINO

    //caminhos geral
    igraph_vs_1(&to[0],ultimo_pixel); // ultimo pixel
    igraph_vs_1(&to[1],primeiro_pixel_ultima);// primeiro pixel ultima camada
    igraph_vs_1(&to[2],primeiro_pixel_ultima_1); // primeiro pixel ultima linha primeira camada
    igraph_vs_1(&to[3],ultimo_pixel_1); // ultimo pixel primeira camada

    //primeira camada
    igraph_vs_1(&to[4],ultimo_pixel_1); // ultimo pixel
    igraph_vs_1(&to[5],primeiro_pixel_ultima_1); // primeiro pixel ultima linha primeira camada
    igraph_vs_1(&to[6],meio_vertical_1); // meio vertical
    igraph_vs_1(&to[7],meio_horizontal_1); // meio horizontal direita]]

    //segunda camada
    igraph_vs_1(&to[8],(ultimo_pixel_1+(image.cols*image.rows))); // ultimo pixel
    igraph_vs_1(&to[9],( primeiro_pixel_ultima_1+(image.cols*image.rows))); //primeiro pixel ultima linha
    igraph_vs_1(&to[10],(meio_vertical_1)+(image.cols*image.rows));
    igraph_vs_1(&to[11],(meio_horizontal_1)+(image.cols*image.rows));

    //terceira camada
    igraph_vs_1(&to[12],(ultimo_pixel_1+(image.cols*image.rows*2))); // ultimo pixel
    igraph_vs_1(&to[13],( primeiro_pixel_ultima_1+(image.cols*image.rows*2))); //primeiro pixel ultima linha
    igraph_vs_1(&to[14],(meio_vertical_1)+(image.cols*image.rows*2));
    igraph_vs_1(&to[15],(meio_horizontal_1)+(image.cols*image.rows*2));

    //CALCULA E IMPRIME MENOR CAMINHO
    for(int i = 0;i < 16;i++)
    {
        igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from[i],to[i],vWeights.getVec(),IGRAPH_ALL,&pred,&inbound);
        avgVector((igraph_vector_t*)VECTOR(ePath)[0],vWeights.getVec(),&res);
    }

    string str_res = print_vector(&res);

    cout << '\n';

    igraph_minimum_spanning_tree_prim(&graph,&mst,vWeights.getVec());

    //DESTRUIÇÃO DOS ELEMENTOS
    igraph_vector_destroy((igraph_vector_t*)VECTOR(vPath)[0]);
    igraph_vector_destroy((igraph_vector_t*)VECTOR(ePath)[0]);
    igraph_vector_ptr_destroy(&vPath);
    igraph_vector_ptr_destroy(&ePath);
    igraph_vector_destroy(&res);
    igraph_destroy(&graph);
    igraph_destroy(&mst);

    return str_res;
}

string atributeGenerator_gray(string arg)
{

    igraph_t graph, mst;
    igraph_vector_t edges,weights,res;

    igraph_vector_ptr_t vPath,ePath;
    igraph_vector_long_t pred,inbound;
    igraph_vs_t to[4];

    Mat image = imread(arg);
    cvtColor(image,image,COLOR_RGB2GRAY);

    const int ultimo_pixel_1  = (image.cols*image.rows) -1;
    const int primeiro_pixel_ultima_1 = (image.cols*image.rows)-image.cols;
    const int meio_vertical_1 = (image.cols*(5/2));
    const int meio_horizontal_1 = ( (image.cols*image.rows)/2 ) + (image.cols-1);

    //INICIALIZAÇÃO DE VETORES
    VectorGraph vEdges((2*((image.channels()*(2*image.cols*image.rows-image.cols-image.rows))+(2*image.rows*image.cols))));
    VectorGraph vWeights((image.channels()*(2*image.cols*image.rows-image.cols-image.rows))+(2*image.rows*image.cols));
    igraph_vector_init(&res,0);
    igraph_vector_ptr_init(&vPath,1);
    igraph_vector_ptr_init(&ePath,1);
    igraph_vector_long_init(&pred,0);
    igraph_vector_long_init(&inbound,0);


    VECTOR(vPath)[0] = calloc(1,sizeof(igraph_vector_t));
    VECTOR(ePath)[0] = calloc(1,sizeof(igraph_vector_t));

    igraph_vector_init((igraph_vector_t*)VECTOR(vPath)[0], 0);
    igraph_vector_init((igraph_vector_t*)VECTOR(ePath)[0], 0);


    graph = createGraph(image);

    EWVector_gray(image,vEdges.getVec(),vWeights.getVec());
    igraph_add_edges(&graph,vEdges.getVec(),0);

    //PIXELS DE PARTIDA
    int from_gray[] {0,(image.cols-1),image.cols/2,image.cols*(image.rows/2)};

    //PIXELS DE DESTINO
    igraph_vs_1(&to[0],ultimo_pixel_1); // ultimo pixel
    igraph_vs_1(&to[1],primeiro_pixel_ultima_1); // primeiro pixel ultima linha primeira camada
    igraph_vs_1(&to[2],meio_vertical_1); // meio vertical
    igraph_vs_1(&to[3],meio_horizontal_1); // meio horizontal direita]]

    //CALCULA E IMPRIME MENOR CAMINHO
    for(int i = 0;i < 4;i++)
    {
        igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from_gray[i],to[i],vWeights.getVec(),IGRAPH_ALL,&pred,&inbound);
        avgVector((igraph_vector_t*)VECTOR(ePath)[0],vWeights.getVec(),&res);
    }

    string str_res = print_vector(&res);

    cout << '\n';

    igraph_minimum_spanning_tree_prim(&graph,&mst,vWeights.getVec());

    //DESTRUIÇÃO DOS ELEMENTOS
    igraph_vector_destroy((igraph_vector_t*)VECTOR(vPath)[0]);
    igraph_vector_destroy((igraph_vector_t*)VECTOR(ePath)[0]);
    igraph_vector_ptr_destroy(&vPath);
    igraph_vector_ptr_destroy(&ePath);
    igraph_vector_destroy(&res);
    igraph_destroy(&graph);
    igraph_destroy(&mst);

    return str_res;

}

int main(int argc, char* argv[])
{
    if(argc != 2)
        exit(-1);

    bool gray_img = true;

    string path = argv[1],str_out;
    ofstream File;
    File.open(path+"/output.arff");

    if(!File.is_open())
        exit(-2);

    if(!gray_img)
    {
            for(int j = 1;j <= 10;j++)
            {
                    int mult = (j-1)*100;
                    for(int i = mult+0;i < mult+100;i++)
                    {
                         std::cerr << "Imagem:  " << i+1<< " de 1000.\n";
                        str_out = atributeGenerator(path+"/class_"+to_string(j)+"/"+to_string(i)+".jpg")+"class_"+to_string(j)+"\n";
                        File << str_out;
                    }
            }
    }
    else
    {
            for(int j = 1;j <= 10;j++)
            {
                    int mult = (j-1)*100;
                    for(int i = mult+0;i < mult+100;i++)
                    {
                         std::cerr << "Imagem:  " << i+1<< " de 1000.\n";
                        str_out = atributeGenerator_gray(path+"/class_"+to_string(j)+"/"+to_string(i)+".jpg")+"class_"+to_string(j)+"\n";
                        File << str_out;
                    }
            }

    }

    File.close();

    return 0;
}
