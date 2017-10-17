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


    // Caso especial bases com nomes nas pastas
        ifstream File;
        File.open("../bases/Kylberg/folders_name.txt");
        if(!File.is_open())
        {
            cerr << "Problema ao Abrir Nomes das Pastas" << '\n';
            exit(EXIT_FAILURE);
        }
        std::vector<string> folders_name;
        std::istream_iterator<string> eos;
        std::istream_iterator<string> input(File);
        std::copy(input,eos,std::back_inserter(folders_name));
     //




    DB(folders_name.size());
    int number_folders = 28, number_images = 160;

    string path = argv[1];
    string image_codec = ".png";


    image_base base{image_codec,path,number_folders,number_images,
                image_base::TYPE::KYLBERG,image_base::COLOR::GRAY};

    if(!base.create_arff_file(argv[2]))
    {
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
    }

    thread_handler(base,folders_name);

}
