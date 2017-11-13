#include <QCoreApplication>
#include <cmath>
#include <istream>
#include <map>
#include "util.cpp"



int main(int argc, char* argv[])
{

    if(argc < 3)
    {
        std::cerr << "Quantidade de argumentos inválida!\n";
        return -1;
    }


    string path = argv[1];

    string image_codec = argv[3];

    if( image_codec.empty() )
    {
        std::cout << "Codec da base não informado!\n";
        return -1;
    }


    std::vector<string> folders_name;

    ifstream File;
    File.open(path+"/folders_name.txt");
    if(!File.is_open())
    {
        cerr << "Problema ao Abrir Nomes das Pastas" << '\n';
        exit(EXIT_FAILURE);
    }

    std::istream_iterator<string> eos;
    std::istream_iterator<string> input(File);
    std::copy_if(input,eos,std::back_inserter(folders_name),[](std::string a) { return a[0] != '#'; });


    int number_folders = folders_name.size(), number_images = 0;


    image_base base{image_codec,path,number_folders,number_images,
        image_base::TYPE::BRODATZ,image_base::COLOR::GRAY};


    if(!base.create_arff_file(argv[2]))
    {
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
    }

    thread_handler(base,folders_name,false);


}
