#include <QCoreApplication>
#include <cmath>
#include <istream>
#include <map>
#include "util.cpp"


void verify_args_number(int argc);


int main(int argc, char* argv[])
{
	
    verify_args_number(argc);

    string folders_path = argv[1];
    bool rgb = std::stoi(argv[5]);
    bool mst = std::stoi(argv[6]);
    string arff_file_path = argv[7];


    image_base base;

    base.path = folders_path;

    if(!rgb)
        base.image_color = COLOR::GRAY;
    else 
        base.image_color = COLOR::RGB;


    base.arff_file.open(arff_file_path);

    if( !base.arff_file.is_open() )
    {
        std::cerr << "Problema ao criar arquivo arff!\n";
        return 1;
    }

    thread_handler(base,mst);


}

void verify_args_number(int argc)
{
    if(argc < 4)
    {
        cerr << "Quantidade de argumentos inválida!\n";

        cerr << "Lista de Argumentos:\n"
             << "1 - Caminho para base de imagens\n" 
             << "2 - Imagens GRAY ou RGB(RGB == 1 | GRAY = 0)\n"
             << "3 - Gerar ou nao MST(1 ou 0)\n"
             << "4 - Destino do arquivo arff(com o nome do arquivo)\n";
   
        exit(EXIT_SUCCESS);
    }

}
