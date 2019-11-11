#include <opencv2/opencv.hpp>
#include <mutex>
#include <queue>
#include <iostream>
#include <functional>
#include <thread>
#include "image_base.cpp"
#include "boost_utils.cpp"
#include "vectorgraph.hpp"

#define DB(X) std::cout << #X << '=' << X << '\n'

constexpr int LIGA_8_CAMADA = 1;

using namespace cv;
using namespace std;

template<COLOR color>
void generate_edges_weights(const Mat &img,igraph_vector_t *edges,igraph_vector_t *weight);

// define os pixels de partida para o algoritmo dijkstra
template<COLOR color>
void destination_pixels(igraph_vs_t* to,const Mat& image);

template<COLOR color>
string atribute_generator(const Mat &image, bool with_mst, bool do_dijkstra=true);

/*
 * Criei essa struct por que achei que pode facilitar na hora de usar a formula
 * mas pode ser que seja desnecessária
 */
template<typename T>
struct pixel_
{
    int x;
    int y;
    T intensity;
};

template<>
void generate_edges_weights<COLOR::RGB>(const Mat &img,igraph_vector_t *edges,igraph_vector_t *weight)
{
    int cont = 0;
    int pixel = 0;
    int wcont = 0;

    Vec3b intensity1,intensity2;

    for(int camada = 0; camada < 3;camada++)
    {
        for(int i = 0;i < img.rows;i++)
        {
            for(int j = 0;j < img.cols;j++,pixel++)
            {
                intensity1 = img.at<Vec3b>(i,j);

                if(camada < 2)
                {
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows);
                    intensity2 = intensity1;
                }
                if((i < img.rows - 1) && (j < img.cols - 1))//PIXEL NÃO É BORDA, LIGA A DIREITA E ABAIXO
                {

                    if constexpr(LIGA_8_CAMADA)// Faz ligação com os oito de cima
                    {
                        if(camada < 2)
                        {
                            intensity1 = img.at<Vec3b>(i,j);
                            VECTOR(*edges)[cont++] = pixel;
                            VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows)+1;
                            intensity2 = img.at<Vec3b>(i,j+1);
                            VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));
                            if(j > 0)
                            {
                                VECTOR(*edges)[cont++] = pixel;
                                VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows)+(img.cols)-1;
                                intensity2 = img.at<Vec3b>(i+1,j-1);
                                VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));
                            }
                            VECTOR(*edges)[cont++] = pixel;
                            VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows)+(img.cols);
                            intensity2 = img.at<Vec3b>(i+1,j);
                            VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));


                            VECTOR(*edges)[cont++] = pixel;
                            VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows)+(img.cols)+1;
                            intensity2 = img.at<Vec3b>(i+1,j+1);
                            VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));
                        }
                    }

                    //LIGA COM O VERTICE DA DIREITA E CALCULA O PESO
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+1;
                    intensity2 = img.at<Vec3b>(i,j + 1);
                    VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    if(j > 0)// NAO FOR BORDA ESQUERDA
                    {
                        //LIGA COM O PIXEL DA DIAGONAL ESQUERDA INFERIOR E CALCULA O PESO
                        VECTOR(*edges)[cont++] = pixel;
                        VECTOR(*edges)[cont++] = pixel+(img.cols)-1;
                        intensity2 = img.at<Vec3b>(i+1,j-1);
                        VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    }
                    //LIGA COM O VERTICE DE BAIXO E CALCULA O PESO
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+(img.cols);
                    intensity2 = img.at<Vec3b>(i+1,j);
                    VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    //LIGA COM O PIXEL DA DIAGONAL DIREITA INFERIOR E CALCULA O PESO
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+(img.cols)+1;
                    intensity2 = img.at<Vec3b>(i+1,j+1);
                    VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));

                }
                else
                {
                    if((i == (img.rows - 1)) && (j == (img.cols - 1)))//ULTIMO PIXEL, NÃO FAZ NADA
                       continue;

                    if(i == (img.rows - 1))//PIXEL NA BORDA INFERIOR, LIGA SÓ A DIREITA
                    {
                        if constexpr(LIGA_8_CAMADA)
                        {
                            if(camada < 2)
                            {
                                VECTOR(*edges)[cont++] = pixel;
                                VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows)+1;
                                intensity2 = img.at<Vec3b>(i,j+1);
                                VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));
                            }
                        }

                        VECTOR(*edges)[cont++] = pixel;
                        VECTOR(*edges)[cont++] = pixel+1;
                        intensity2 = img.at<Vec3b>(i,j + 1);
                        VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    }
                    else if(j == (img.cols - 1))
                    {
                        if constexpr(LIGA_8_CAMADA)
                        {
                            if(camada < 2)
                            {
                                VECTOR(*edges)[cont++] = pixel;
                                VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows)+(img.cols)-1;
                                intensity2 = img.at<Vec3b>(i+1,j-1);
                                VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));

                                VECTOR(*edges)[cont++] = pixel;
                                VECTOR(*edges)[cont++] = pixel+(img.cols*img.rows)+(img.cols);
                                intensity2 = img.at<Vec3b>(i+1,j);
                                VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada+1]));
                            }
                        }
                        // LIGA NA DIAGONAL INFERIOR ESQUERDA
                        VECTOR(*edges)[cont++] = pixel;
                        VECTOR(*edges)[cont++] = pixel+(img.cols)-1;
                        intensity2 = img.at<Vec3b>(i+1,j-1);
                        VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                        //PIXEL NA BORDA DIREITA, LIGA ABAIXO
                        VECTOR(*edges)[cont++] = pixel;
                        VECTOR(*edges)[cont++] = pixel+(img.cols);
                        intensity2 = img.at<Vec3b>(i+1,j);
                        VECTOR(*weight)[wcont++] = abs((int)(intensity1.val[camada] - intensity2.val[camada]));
                    }
                }
            }
        }
    }


}

template<>
void generate_edges_weights<COLOR::GRAY>(const Mat &img,igraph_vector_t *edges,igraph_vector_t *weight)
{
    int cont = 0,pixel = 0,wcont = 0;

    for(int i = 0; i < img.rows;i++)
    {
        for(int j = 0;j < img.cols;j++,pixel++)
        {

            if((i < img.rows - 1) && (j < img.cols - 1))//PIXEL NÃO É BORDA, LIGA A DIREITA E ABAIXO E DIAGONAIS
            {
                //LIGA COM O VERTICE DA DIREITA E CALCULA O PESO
                VECTOR(*edges)[cont++] = pixel;
                VECTOR(*edges)[cont++] = pixel+1;
                VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i,j + 1)));
                if(j > 0)
                {
                    //LIGA COM A DIAGONAL INFERIOR ESQUERDA E CALCULA O PESO
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+(img.cols)-1;
                    VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j-1)));
                }
                //LIGA COM O VERTICE DE BAIXO E CALCULA O PESO
                VECTOR(*edges)[cont++] = pixel;
                VECTOR(*edges)[cont++] = pixel+(img.cols);
                VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j)));
                //LIGA COM A DIAGONAL INFERIOR DIREITA E CALCULA O PESO
                VECTOR(*edges)[cont++] = pixel;
                VECTOR(*edges)[cont++] = pixel+(img.cols)+1;
                VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j+1)));

            }
            else
            {
                if((i == (img.rows - 1)) && (j == (img.cols - 1)))//ULTIMO PIXEL, NÃO FAZ NADA
                    continue;

                if(i == (img.rows - 1))//PIXEL NA BORDA INFERIOR, LIGA SÓ A DIREITA
                {
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+1;
                    VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i,j + 1)));
                }
                else if(j == (img.cols - 1))
                {
                    //LIGA NA DIAGONAL ESQUERDA INFERIOR
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+(img.cols)-1;
                    VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j-1)));
                    //PIXEL NA BORDA DIREITA LIGA ABAIXO
                    VECTOR(*edges)[cont++] = pixel;
                    VECTOR(*edges)[cont++] = pixel+(img.cols);
                    VECTOR(*weight)[wcont++] = abs(((int)img.at<uchar>(i,j) - (int)img.at<uchar>(i+1,j)));

                }
            }
        }
    }
}

// gera uma string contendo os valores de média e desvio padrão do vetor(v)
string str_arff(igraph_vector_t *v)
{
  long l = igraph_vector_size(v);

  string res{""};

  for (long i = 0; i < l; i += 2)
    res += to_string((double) VECTOR(*v)[i]) + "," + to_string((double) VECTOR(*v)[i+1])+",";

  return res;
}

// cria um grafo a partir da imagem passada
igraph_t createGraph(const Mat &imagem)
{
    igraph_t graph;

    igraph_integer_t n = imagem.rows * imagem.cols * imagem.channels();
    igraph_empty(&graph,n,IGRAPH_UNDIRECTED);


    return graph;
}

template<>
void destination_pixels<COLOR::GRAY>(igraph_vs_t* to,const Mat& image)
{

    const int ultimo_pixel_1  = (image.cols*image.rows) -1;
    const int primeiro_pixel_ultima_1 = (image.cols*image.rows)-image.cols;
    const int meio_vertical_1 = (image.cols*(5/2));
    const int meio_horizontal_1 = ((image.cols*(image.rows/2)) + (image.cols-1));

    //PIXELS DE DESTINO
    igraph_vs_1(&to[0],ultimo_pixel_1); // ultimo pixel
    igraph_vs_1(&to[1],primeiro_pixel_ultima_1); // primeiro pixel ultima linha primeira camada
    igraph_vs_1(&to[2],meio_vertical_1); // meio vertical
    igraph_vs_1(&to[3],meio_horizontal_1); // meio horizontal direita

}

template<>
void destination_pixels<COLOR::RGB>(igraph_vs_t* to,const Mat& image)
{
    const int ultimo_pixel = (  (image.cols*image.rows)-1)+(image.cols*image.rows*2) ;
    const int primeiro_pixel_ultima = ((image.cols*image.rows)-image.cols)+(image.cols*image.rows*2);
    const int ultimo_pixel_1  = (image.cols*image.rows) -1;
    const int primeiro_pixel_ultima_1 = (image.cols*image.rows)-image.cols;
    const int meio_vertical_1 = (image.cols*(5/2));
    const int meio_horizontal_1 = ( (image.cols*(image.rows/2) ) + (image.cols-1));

    //PIXELS DE DESTINO

    //caminhos geral
    igraph_vs_1(&to[0],ultimo_pixel); // ultimo pixel
    igraph_vs_1(&to[1],primeiro_pixel_ultima);// primeiro pixel ultima camada
    igraph_vs_1(&to[2],primeiro_pixel_ultima_1); // primeiro pixel ultima linha primeira camada
    igraph_vs_1(&to[3],ultimo_pixel_1); // ultimo pixel primeira camada

    //primeira camada
    igraph_vs_1(&to[4],ultimo_pixel_1);
    igraph_vs_1(&to[5],primeiro_pixel_ultima_1); // primeiro pixel ultima linha primeira camada
    igraph_vs_1(&to[6],meio_vertical_1); // meio vertical
    igraph_vs_1(&to[7],meio_horizontal_1); // meio horizontal direita

    //segunda camada
    igraph_vs_1(&to[8],(ultimo_pixel_1+(image.cols*image.rows)));
    igraph_vs_1(&to[9],( primeiro_pixel_ultima_1+(image.cols*image.rows))); //primeiro pixel ultima linha
    igraph_vs_1(&to[10],(meio_vertical_1)+(image.cols*image.rows));
    igraph_vs_1(&to[11],(meio_horizontal_1)+(image.cols*image.rows));

    //terceira camada
    igraph_vs_1(&to[12],(ultimo_pixel_1+(image.cols*image.rows*2))); // ultimo pixel
    igraph_vs_1(&to[13],( primeiro_pixel_ultima_1+(image.cols*image.rows*2))); //primeiro pixel ultima linha
    igraph_vs_1(&to[14],(meio_vertical_1)+(image.cols*image.rows*2));
    igraph_vs_1(&to[15],(meio_horizontal_1)+(image.cols*image.rows*2));

}

//gera um vetor resultante que contem as médias e os desvios padrões das distâncias
void avgVector(igraph_vector_t *edges,igraph_vector_t *weights, igraph_vector_t *res)
{
    igraph_real_t sum = 0,des = 0;
    const auto size = igraph_vector_size(edges);

    for(int i = 0; i < size;i++)
    {
        sum += (igraph_real_t)VECTOR(*weights)[(int)VECTOR(*edges)[i]];
    }

    sum /= size;
    igraph_vector_push_back(res,sum);

    for(int i = 0; i < size;i++)
    {
        igraph_real_t temp = ((igraph_real_t)VECTOR(*weights)[(int)VECTOR(*edges)[i]])-sum ;
        des += temp * temp; 
    }

    des /= size;
    des = sqrt(des);

    igraph_vector_push_back(res,des);
}

template<>
string atribute_generator<COLOR::GRAY>(const Mat &image, bool with_mst, bool do_dijkstra)
{
        string str_res;

        igraph_t graph;

        igraph_vector_ptr_t vPath,ePath;

        const int from[] = {0,(image.cols-1),image.cols/2,image.cols*(image.rows/2)};
        igraph_vs_t to[4];

        destination_pixels<COLOR::GRAY>(to,image);

        /*
        if(image.channels() == 3)
            cvtColor(image,image,COLOR_RGB2GRAY);
            */


        const int h_edges = image.rows*(image.cols-1);
        const int v_edges = image.cols*(image.rows-1);
        const int diag_edges = 2*(image.rows*(image.cols-1)) - (2*(image.cols-1));// Para 8 vizinhos

        //INICIALIZAÇÃO DE VETORES
        VectorGraph vEdges(2*(h_edges + v_edges + diag_edges));
        VectorGraph vWeights((h_edges + v_edges + diag_edges));
        VectorGraph res;
        VectorGraph edges_mst;

        igraph_vector_ptr_init(&vPath,1);
        igraph_vector_ptr_init(&ePath,1);


        VECTOR(vPath)[0] = calloc(1,sizeof(igraph_vector_t));
        VECTOR(ePath)[0] = calloc(1,sizeof(igraph_vector_t));

        igraph_vector_init((igraph_vector_t*)VECTOR(vPath)[0], 0);
        igraph_vector_init((igraph_vector_t*)VECTOR(ePath)[0], 0);

        graph = createGraph(image);

        generate_edges_weights<COLOR::GRAY>(image,&vEdges,&vWeights);

       igraph_add_edges(&graph,&vEdges,0);

        //CALCULA E IMPRIME MENOR CAMINHO
        if(do_dijkstra)
        {
            for(int i = 0;i < 4;i++)
            {
                igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from[i],to[i],&vWeights,IGRAPH_ALL,NULL,NULL);
                avgVector((igraph_vector_t*)VECTOR(ePath)[0],&vWeights,&res);
            }
        }

        if(with_mst)
        {
            igraph_minimum_spanning_tree(&graph,&edges_mst,&vWeights);

            avgVector(&edges_mst,&vWeights,&res);
        }

        str_res = str_arff(&res);


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

template<>
string atribute_generator<COLOR::RGB>(const Mat &image, bool with_mst, bool do_dijkstra)
{
        // STRING de atributos e string pra cada camada
        string str_res,layer_str;

        str_res.reserve(40*8+41);
        layer_str.reserve(18);


        igraph_t graph;

        igraph_vector_ptr_t vPath,ePath;

        igraph_vs_t to[16];

        const int from[] = {0,(image.cols-1),(image.cols-1 + image.cols*image.rows * 2),(image.cols*image.rows*2),
            0,(image.cols-1),image.cols/2,image.cols*(image.rows/2),
              image.cols*image.rows,(image.cols-1)+(image.cols*image.rows),(image.cols/2)+(image.cols*image.rows),image.cols*(image.rows/2)+(image.cols*image.rows),
              image.cols*image.rows*2,(image.cols-1)+(image.cols*image.rows*2),(image.cols/2)+(image.cols*image.rows*2),image.cols*(image.rows/2)+(image.cols*image.rows*2)};

        destination_pixels<COLOR::RGB>(to,image);

        const int h_edges = image.rows*(image.cols-1);
        const int v_edges = image.cols*(image.rows-1);
        const int diag_edges = 2*(image.rows*(image.cols-1)) - (2*(image.cols-1));// Para 8 vizinhos
        int camada_8 = 1/2;

        if constexpr( LIGA_8_CAMADA )
        {
            camada_8 = (h_edges + v_edges + diag_edges);
        }

        //INICIALIZAÇÃO DE VETORES
        VectorGraph vEdges(2*(image.channels()*(h_edges + v_edges + diag_edges)+(2*image.rows*image.cols)+(2*camada_8)));
        VectorGraph vWeights(image.channels()*(h_edges + v_edges + diag_edges)+(2*image.rows*image.cols)+(2*camada_8));
        VectorGraph res;
        VectorGraph edges_mst;

        igraph_vector_ptr_init(&vPath,1);
        igraph_vector_ptr_init(&ePath,1);


        VECTOR(vPath)[0] = calloc(1,sizeof(igraph_vector_t));
        VECTOR(ePath)[0] = calloc(1,sizeof(igraph_vector_t));

        igraph_vector_init((igraph_vector_t*)VECTOR(vPath)[0], 0);
        igraph_vector_init((igraph_vector_t*)VECTOR(ePath)[0], 0);


        graph = createGraph(image);

        generate_edges_weights<COLOR::RGB>(image,&vEdges,&vWeights);

        igraph_add_edges(&graph,&vEdges,0);

        //CALCULA E IMPRIME MENOR CAMINHO
        if(do_dijkstra)
        {
            for(int i = 0; i < 16;i++)
            {
                igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from[i],to[i],&vWeights,IGRAPH_ALL,NULL,NULL);
                avgVector((igraph_vector_t*)VECTOR(ePath)[0],&vWeights,&res);
            }
        }

        if(with_mst)
        {
            igraph_minimum_spanning_tree(&graph,&edges_mst,&vWeights);

            avgVector(&edges_mst,&vWeights,&res);

            Mat layers[3];
            split(image,layers);

            // FAZ MST PARA CADA CAMADA R G e B
            for(int i = 0; i < 3; i++)
            {
                layer_str += atribute_generator<COLOR::GRAY>(layers[i],with_mst,false);

            }
        }

        str_res = str_arff(&res);


        //DESTRUIÇÃO DOS ELEMENTOS
        igraph_vector_destroy((igraph_vector_t*)VECTOR(vPath)[0]);
        igraph_vector_destroy((igraph_vector_t*)VECTOR(ePath)[0]);
        igraph_vector_ptr_destroy(&vPath);
        igraph_vector_ptr_destroy(&ePath);
        igraph_destroy(&graph);

        for(int i = 0 ; i < 16; i++)
          igraph_vs_destroy(&to[i]);


        return str_res+layer_str;

}

string produce_values(string_view arg,image_base& base,bool with_mst,bool do_dijkstra)
{
    const Mat image = imread(arg.data());


    if(base.image_color == COLOR::RGB)
    {
        return atribute_generator<COLOR::RGB>(image,with_mst,do_dijkstra);
    }

    return atribute_generator<COLOR::GRAY>(image,with_mst,do_dijkstra);

}

mutex mt;
void extrai_valor(string_view folder,image_base& base,bool with_mst,bool do_dijkstra)
{

    stringstream values;

    const vector<string> images_path = get_images_in_class(bsf::path(base.path+'/'+std::string(folder.data())));

    for(int i = 0; i < images_path.size(); i++)
    {
        std::cout << "Thread: " << folder << "\nImagem: " << i << " de " << images_path.size() << '\n';

        string temp = produce_values(images_path[i],base,with_mst,do_dijkstra);

        temp += "class_"+std::string(folder.data())+"\n";

        values << temp;
    }

    mt.lock();
    base.arff_file << values.str();
    mt.unlock();


}

void thread_handler(image_base& base,bool do_dijkstra,bool with_mst)
{
    const int num_threads = 4;
    std::vector<thread> threads;

    const std::vector<string> folders_name = get_classes_name( bsf::path(base.path));

    base.put_classes_in_arff(folders_name);

    threads.reserve(folders_name.size());

    int counter = 0;
    float percentage = 0.0f;
    for(int i = 0; i < folders_name.size(); )
    {

        int antigo_i = i;
        for(int j = 0; j < num_threads && i < folders_name.size(); j++,i++)
        {
            counter++;
            threads.emplace_back(extrai_valor,folders_name[i],std::ref(base),with_mst,do_dijkstra);
        }
        percentage = (counter/folders_name.size()) * 100;

        std::cout << percentage << "% complete!\n";
        for(int j = antigo_i; j < i;j++)
            threads[j].join();


    }


}
