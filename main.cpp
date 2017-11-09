#include <QCoreApplication>
#include <cmath>
#include <istream>
#include "util.cpp"



int main(int argc, char* argv[])
{

    if(argc < 2)
    {
        std::cerr << "Quantidade de argumentos inválida!\n";
        exit(-1);
    }


    string path = argv[1];

    // Pra lembrar de configurar os argumentos
    std::cout << "Caminho para Base que será utilizada:\n\t" << path << "\nNome do arquivo ARFF:\n\t"<< argv[2] << '\n';
    std::cout << "Continuar?\n";
    std::cin.get();

    // Caso especial bases com nomes nas pastas
        ifstream File;
<<<<<<< HEAD
        File.open(path+"/folders_name.txt");
=======
        File.open("../home/rodrigo/ic/bases/UCMerced_LandUse/Images/folders_name.txt");
>>>>>>> ab22a3613354674ae814fb61befd0320d3b5a6ee
        if(!File.is_open())
        {
            cerr << "Problema ao Abrir Nomes das Pastas" << '\n';
            exit(EXIT_FAILURE);
        }
        std::vector<string> folders_name;
        std::istream_iterator<string> eos;
        std::istream_iterator<string> input(File);
        std::copy_if(input,eos,std::back_inserter(folders_name),[](std::string a) { return a[0] != '#'; });
     //




<<<<<<< HEAD
    //CALCULA E IMPRIME MENOR CAMINHO
    for(int i = 0;i < 16;i++)
    {
        igraph_get_shortest_paths_dijkstra(&graph,&vPath,&ePath,from[i],to[i],vWeights.getVec(),IGRAPH_ALL,&pred,&inbound);
        avgVector((igraph_vector_t*)VECTOR(ePath)[0],vWeights.getVec(),&res);
    }
    igraph_minimum_spanning_tree(&graph,&edges_mst,vWeights.getVec());
    avgVector(&edges_mst,vWeights.getVec(),&res);


    igraph_minimum_spanning_tree(&graph,&edges_mst,vWeights.getVec());
    avgVector(&edges_mst,vWeights.getVec(),&res);

    string str_res = print_vector(&res);

    cout << '\n';


    //DESTRUIÇÃO DOS ELEMENTOS
    igraph_vector_destroy((igraph_vector_t*)VECTOR(vPath)[0]);
    igraph_vector_destroy((igraph_vector_t*)VECTOR(ePath)[0]);
    igraph_vector_ptr_destroy(&vPath);
    igraph_vector_ptr_destroy(&ePath);
    igraph_vector_destroy(&res);
    igraph_destroy(&graph);
    igraph_vector_destroy(&edges_mst);

    return str_res;
}

string atributeGenerator_gray(string arg)
{

    igraph_t graph;
    igraph_vector_t res,edges_mst;

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
    igraph_vector_init(&edges_mst,0);
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

<<<<<<< HEAD

    cout << '\n';

    igraph_minimum_spanning_tree(&graph,&edges_mst,vWeights.getVec());
    int size = igraph_vector_size(&edges_mst);


    avgVector(&edges_mst,vWeights.getVec(),&res);

    string str_res = print_vector(&res);
=======
//    igraph_minimum_spanning_tree(&graph,&edges_mst,vWeights.getVec());

 //   avgVector(&edges_mst,vWeights.getVec(),&res);
    string str_res = print_vector(&res);

    cout << '\n';
=======
    DB(folders_name.size());
<<<<<<< HEAD
    // temporario
    int number_folders = folders_name.size(), number_images = 400;
>>>>>>> f5a5060a85de29bdf055aadf25cd7573fc94387d


>>>>>>> master

=======

    int number_folders = folders_name.size(), number_images = 160;

    string path = argv[1];
>>>>>>> ab22a3613354674ae814fb61befd0320d3b5a6ee
    string image_codec = ".tif";


    image_base base{image_codec,path,number_folders,number_images,
<<<<<<< HEAD
                image_base::TYPE::RGBBRODATZ,image_base::COLOR::RGB};
=======
                image_base::TYPE::UCM,image_base::COLOR::RGB};
>>>>>>> ab22a3613354674ae814fb61befd0320d3b5a6ee

    if(!base.create_arff_file(argv[2]))
    {
<<<<<<< HEAD
            for(int j = 1;j <= 10;j++)
            {
                    int mult = (j-1)*100;
                    for(int i = mult+0;i < mult+100;i++)
                    {
                         std::cout << "Imagem:  " << i+1<< " de 1000.\n";
                        str_out = atributeGenerator(path+"/"+path_folder+to_string(j)+"/"+to_string(i)+image_codec)+path_folder+to_string(j)+"\n";
                        File << str_out;
                    }
            }
    }
    else
    {
        if(!brodatz)
        {

            for(int j = 1;j <= 10;j++)
            {
                    int mult = (j-1)*100;
                    for(int i = mult+0;i < mult+100;i++)
                    {
<<<<<<< HEAD
                         std::cerr << "Imagem:  " << i+1<< " de 1000.\n";
                        str_out = atributeGenerator_gray(path+"/class_"+to_string(j)+"/"+to_string(i)+".jpg")+"class_"+to_string(j)+"\n";
                         std::cerr << "OUTPUT ARFF:  " << str_out << '\n';
=======
                         std::cout << "Imagem:  " << i+1<< " de 1000.\n";
                        str_out = atributeGenerator_gray(path+"/"+path_folder+to_string(j)+"/"+to_string(i)+image_codec)+path_folder+to_string(j)+"\n";
                        cerr << "ARFF OUTPUT: " << str_out << '\n';
>>>>>>> master
                        File << str_out;
                    }
            }
         }
         else
        {
            for(int j = 1; j <= 13; j++)
            {

                for(int i = 0; i < 256/* 1024 terceira classe em diante*/; i++)
                {

                        std::cout << "Imagem BROADTZ:  " << i+1<< " de 256 CLASSE: " << j << '\n';
                        str_out = atributeGenerator_gray(path+"/"+path_folder+to_string(j)+"/"+"output"+to_string(j)+"_"+to_string(i)+image_codec);
                        str_out += path_folder+to_string(j)+"\n";

                        File << str_out;
                }


            }



        }

=======
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
>>>>>>> f5a5060a85de29bdf055aadf25cd7573fc94387d
    }

    thread_handler(base,folders_name,false);

}
