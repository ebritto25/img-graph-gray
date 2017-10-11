#include <opencv2/opencv.hpp>
#include "image_base.h"
#include <mutex>
#include <array>
#include <iostream>
#include <functional>
#include <thread>
#include "vectorgraph.hpp"

#define DB(X) std::cerr << #X << '=' << X << '\n';

using namespace cv;
using namespace std;

// gera uma matriz de adjacência a partir de uma dada imagem(image)
void gera_mat_adj(igraph_matrix_t* mat_adj,Mat& image,igraph_vector_t* weights)
{

	igraph_matrix_init(mat_adj,0,0);
	igraph_matrix_add_cols(mat_adj,image.cols*image.rows);
	igraph_matrix_add_rows(mat_adj,image.cols*image.rows);


	for(int i = 0; i < image.rows*image.cols;i++)
		for(int j = 0; j < image.rows*image.cols;j++)
			MATRIX(*mat_adj,i,j) = image.at<uchar>(i/image.cols,i%image.rows) - image.at<uchar>(j/image.cols,j%image.rows);

	int weight_index = 0;

	for(int i = 0; i < image.rows*image.cols;i++)
	{
        for(int j = i+1; j < image.rows*image.cols-1;j++)
		{
		    VECTOR(*weights)[weight_index] = MATRIX(*mat_adj,i,j);
		}
		weight_index++;
	}

}


// mostra a imagem(img)
void mostra_img(Mat img)
{
    namedWindow("Imagem");
    imshow("Imagem",img);
    waitKey(0);
}

// gera uma string contendo os valores de média
// e desvio padrão do vetor (v)
string gera_vector_arff(igraph_vector_t *v)
{

  long int i, l = igraph_vector_size(v);

  string res {""};

  for (i=0; i<l; i += 2)
    res += to_string((double) VECTOR(*v)[i]) + "," + to_string((double) VECTOR(*v)[i+1])+",";

  return res;
}

// cria um grafo a partir da imagem
// passada
igraph_t createGraph(Mat &imagem)
{
    igraph_t graph;

    igraph_integer_t n = imagem.rows * imagem.cols * imagem.channels();
    igraph_empty(&graph,n,IGRAPH_UNDIRECTED);

    cout << "Criado grafo "<< igraph_vcount(&graph) << " nós\n";
    cout << "Imagem: "<< imagem.cols << 'x' << imagem.rows << '\n';

    return graph;
}


// define os pixels de partida
// para o dijkstra
template <typename T>
void define_pixels_destino(T& to,Mat& image,image_base::COLOR color)
{
    if(color == image_base::COLOR::GRAY)
    {
        const int ultimo_pixel_1  = (image.cols*image.rows) -1;
        const int primeiro_pixel_ultima_1 = (image.cols*image.rows)-image.cols;
        const int meio_vertical_1 = (image.cols*(5/2));
        const int meio_horizontal_1 = ((image.cols*image.rows)/2) + (image.cols-1);

        //PIXELS DE DESTINO
        igraph_vs_1(&to[0],ultimo_pixel_1); // ultimo pixel
        igraph_vs_1(&to[1],primeiro_pixel_ultima_1); // primeiro pixel ultima linha primeira camada
        igraph_vs_1(&to[2],meio_vertical_1); // meio vertical
        igraph_vs_1(&to[3],meio_horizontal_1); // meio horizontal direita]]


    }
    else
    {
        const int ultimo_pixel = (  (image.cols*image.rows)-1)+(image.cols*image.rows*2) ;
        const int primeiro_pixel_ultima = ((image.cols*image.rows)-image.cols)+(image.cols*image.rows*2);
        const int ultimo_pixel_1  = (image.cols*image.rows) -1;
        const int primeiro_pixel_ultima_1 = (image.cols*image.rows)-image.cols;
        const int meio_vertical_1 = (image.cols*(5/2));
        const int meio_horizontal_1 = ( (image.cols*image.rows)/2 ) + (image.cols-1);

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

    }

}

// gera um vetor de edges e de weight
// dada uma imagem (img) em tons de cinza
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

// gera um vetor de edges e de weight
// dada uma imagem (img)
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

//gera um vetor resultante que contem as médias
// e os desvios padrões das distâncias
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
        igraph_real_t temp = ((igraph_real_t)VECTOR(*weights)[(int)VECTOR(*edges)[i]])-sum ;
        des += temp * temp; // Melhor que usar Pow
    }

    des /= size;
    des = sqrt(des);

    igraph_vector_push_back(res,des);
}

//gera o arquivo arff de uma determinada imagem colorida
string atributeGenerator(string arg,image_base& base)
{
    igraph_t graph;

    igraph_vector_ptr_t vPath,ePath;



    igraph_vs_t to[16];

    Mat image = imread(arg);
    int from[] = {0,(image.cols-1),(image.cols-1 + image.cols*image.rows * 2),(image.cols*image.rows*2),
        0,(image.cols-1),image.cols/3,image.cols*(image.rows/2),
          image.cols*image.rows,(image.cols-1)+(image.cols*image.rows),(image.cols/2)+(image.cols*image.rows),image.cols*(image.rows/2)+(image.cols*image.rows),
          image.cols*image.rows*2,(image.cols-1)+(image.cols*image.rows*2),(image.cols/2)+(image.cols*image.rows*2),image.cols*(image.rows/2)+(image.cols*image.rows*2)};

    define_pixels_destino(to,image,base.color());

    //INICIALIZAÇÃO DE VETORES
    VectorGraph vEdges((2*((image.channels()*(2*image.cols*image.rows-image.cols-image.rows))+(2*image.rows*image.cols))));
    VectorGraph vWeights((image.channels()*(2*image.cols*image.rows-image.cols-image.rows))+(2*image.rows*image.cols));
    VectorGraph res;
    VectorGraph edges_mst;

    igraph_vector_ptr_init(&vPath,1);
    igraph_vector_ptr_init(&ePath,1);



    VECTOR(vPath)[0] = calloc(1,sizeof(igraph_vector_t));
    VECTOR(ePath)[0] = calloc(1,sizeof(igraph_vector_t));

    igraph_vector_init((igraph_vector_t*)VECTOR(vPath)[0], 0);
    igraph_vector_init((igraph_vector_t*)VECTOR(ePath)[0], 0);


    graph = createGraph(image);

    EWVector(image,&vEdges,&vWeights);
    
    igraph_add_edges(&graph,&vEdges,0);

    //CALCULA E IMPRIME MENOR CAMINHO
    for(int i = 0;i < 16;i++)
    {
        igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from[i],to[i],&vWeights,IGRAPH_ALL,NULL,NULL);
        avgVector((igraph_vector_t*)VECTOR(ePath)[0],&vWeights,&res);
    }

    igraph_minimum_spanning_tree(&graph,&edges_mst,&vWeights);
    avgVector(&edges_mst,&vWeights,&res);
    string str_res = gera_vector_arff(&res);
    cout << '\n';


    //DESTRUIÇÃO DOS ELEMENTOS
    igraph_vector_destroy((igraph_vector_t*)VECTOR(vPath)[0]);
    igraph_vector_destroy((igraph_vector_t*)VECTOR(ePath)[0]);
    igraph_vector_ptr_destroy(&vPath);
    igraph_vector_ptr_destroy(&ePath);
    igraph_destroy(&graph);

    for(int i = 0 ; i < 16; i++)
      igraph_vs_destroy(&to[i]);

    return str_res;
}

//gera o arquivo arff de uma determinada imagem
// em tons de cinza
string atributeGenerator_gray(string arg,image_base& base)
{

    igraph_t graph;
    igraph_vector_ptr_t vPath,ePath;

    Mat image = imread(arg);

    int from[] = {0,(image.cols-1),image.cols/2,image.cols*(image.rows/2)};
    igraph_vs_t to[4];

    define_pixels_destino(to,image,base.color());

    cvtColor(image,image,COLOR_RGB2GRAY);

    //INICIALIZAÇÃO DE VETORES
    VectorGraph vEdges((2*((image.channels()*(2*image.cols*image.rows-image.cols-image.rows))+(2*image.rows*image.cols))));
    VectorGraph vWeights((2*image.cols*image.rows-image.cols-image.rows)+(2*image.rows*image.cols));
    VectorGraph res;
    VectorGraph edges_mst;

    igraph_vector_ptr_init(&vPath,1);
    igraph_vector_ptr_init(&ePath,1);


    VECTOR(vPath)[0] = calloc(1,sizeof(igraph_vector_t));
    VECTOR(ePath)[0] = calloc(1,sizeof(igraph_vector_t));

    igraph_vector_init((igraph_vector_t*)VECTOR(vPath)[0], 0);
    igraph_vector_init((igraph_vector_t*)VECTOR(ePath)[0], 0);

    graph = createGraph(image);



    EWVector_gray(image,&vEdges,&vWeights);
    igraph_add_edges(&graph,&vEdges,0);

    //CALCULA E IMPRIME MENOR CAMINHO
    for(int i = 0;i < 4;i++)
    {
        igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from[i],to[i],&vWeights,IGRAPH_ALL,NULL,NULL);
        avgVector((igraph_vector_t*)VECTOR(ePath)[0],&vWeights,&res);
    }

    igraph_minimum_spanning_tree(&graph,&edges_mst,&vWeights);

    avgVector(&edges_mst,&vWeights,&res);

    string str_res = gera_vector_arff(&res);

    //cout << '\n';



    //DESTRUIÇÃO DOS ELEMENTOS
    igraph_vector_destroy((igraph_vector_t*)VECTOR(vPath)[0]);
    igraph_vector_destroy((igraph_vector_t*)VECTOR(ePath)[0]);
    igraph_vector_ptr_destroy(&vPath);
    igraph_vector_ptr_destroy(&ePath);
    igraph_destroy(&graph);

    for(int i = 0 ; i < 4; i++)
      igraph_vs_destroy(&to[i]);

    return str_res;

}

std::mutex mt;
// extrai valores de uma dada base de imagem
void extrai_valor(int folder,image_base& base)
{
    Mat image = imread(base.get_image_in_folder(folder,base.get_image_base_type(),0));

    stringstream values;

    if(base.color() == image_base::COLOR::RGB )
    {


        for(int i = 0; i < base.images(); i++)
        {
            std::cerr << "Thread: " << folder << "\nImagem: " << i << " de " << base.images() << '\n';
            string img_str  = base.get_image_in_folder(folder,base.get_image_base_type(),i);

            string temp = atributeGenerator(img_str,base);
            temp += "class_"+to_string(folder)+"\n";

            values << temp;
        }

    }
    else
    {

        for(int i = 0; i < base.images(); i++)
        {
            std::cerr << "Thread: " << folder << "\nImagem: " << i << " de " << base.images() << '\n';
            string img_str = base.get_image_in_folder(folder,base.get_image_base_type(),i);

            string temp = atributeGenerator_gray(img_str,base);
            temp += "class_"+to_string(folder)+"\n";

            values << temp;
        }


    }


    mt.lock();
    base.put_in_arff_file(values.str());
    mt.unlock();

}

void extrai_valor_str(string folder,image_base& base)
{
    Mat image = imread(base.get_image_in_folder(folder,base.get_image_base_type(),0));

    stringstream values;

    if(base.color() == image_base::COLOR::RGB )
    {


        for(int i = 0; i < base.images(); i++)
        {
            std::cerr << "Thread: " << folder << "\nImagem: " << i << " de " << base.images() << '\n';
            string img_str  = base.get_image_in_folder(folder,base.get_image_base_type(),i);

            string temp = atributeGenerator(img_str,base);
            temp += "class_"+folder+"\n";

            values << temp;
        }


    }
    else
    {


        for(int i = 0; i < base.images(); i++)
        {
            std::cerr << "Thread: " << folder << "\nImagem: " << i << " de " << base.images() << '\n';
            string img_str = base.get_image_in_folder(folder,base.get_image_base_type(),i);

            string temp = atributeGenerator_gray(img_str,base);
            temp += "class_"+folder+"\n";

            values << temp;
        }


    }


    mt.lock();
    base.put_in_arff_file(values.str());
    mt.unlock();

}
void thread_handler(image_base& base)
{
    std::vector<thread> threads;

    threads.reserve(base.folders());

    for(int i = 1; i <= base.folders(); i++)
        threads.emplace_back(extrai_valor,i,std::ref(base));

    for(int i = 0; i < base.folders(); i++)
        threads[i].join();

}
void thread_handler(image_base& base,std::vector<string> folders)
{
    std::vector<thread> threads;

    threads.reserve(base.folders());

    for(int i = 0; i < base.folders(); i++)
        threads.emplace_back(extrai_valor_str,folders[i],std::ref(base));


    for(int i = 0; i < base.folders(); i++)
        threads[i].join();
}
