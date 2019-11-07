#include <QCoreApplication>
#include <cmath>
#include <istream>
#include <array>
#include <string_view>
#include <map>
#include "functions.cpp"


void verify_args_number(int argc);


int main(int argc, char* argv[])
{
	
    verify_args_number(argc);

    std::string_view folders_path = argv[1];
    const bool rgb = std::stoi(argv[2]);
    const bool do_dijkstra = std::stoi(argv[3]);
    const bool mst = std::stoi(argv[4]);
    const std::string arff_file_path = argv[5];

    image_base base;

    base.path = folders_path.data();

    std::string dataset_name = get_base_naem(bsf::path(base.path));

    std::string color{"GRAY"};
    if(!rgb)
        base.image_color = COLOR::GRAY;
    else 
    {
        color = "RGB";
        base.image_color = COLOR::RGB;
    }

    const std::string arff_name{dataset_name+"_"+color+"_"+(do_dijkstra?"DIJ_":"")+(mst?"MST_":"")+".arff"};

    base.arff_file.open(arff_file_path+arff_name);

    std::cout << arff_file_path + arff_name <<'\n';

    if( !base.arff_file.is_open() )
    {
        std::cerr << "Problema ao criar arquivo arff!\n";
        return 1;
    }

    base.init_arff_file(do_dijkstra,mst,rgb);

    thread_handler(base,do_dijkstra,mst);


}

void verify_args_number(int argc)
{
    if(argc < 4)
    {
        cerr << "Quantidade de argumentos inválida!\n";

        cerr << "Lista de Argumentos:\n"
             << "1 - Caminho para base de imagens\n" 
             << "2 - Imagens GRAY ou RGB(RGB == 1 | GRAY = 0)\n"
             << "3 - Fazer o dijkstra ou não(1 ou 0)\n"
             << "4 - Gerar ou nao MST(1 ou 0)\n"
             << "5 - Destino do arquivo arff\n";
   
        exit(EXIT_FAILURE);
    }

}
