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
        File.open(path+"/folders_name.txt");
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




    DB(folders_name.size());
    // temporario
    int number_folders = folders_name.size(), number_images = 400;



    string image_codec = ".tif";


    image_base base{image_codec,path,number_folders,number_images,
                image_base::TYPE::RGBBRODATZ,image_base::COLOR::RGB};

    if(!base.create_arff_file(argv[2]))
    {
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
    }

    thread_handler(base,folders_name,false);

}
