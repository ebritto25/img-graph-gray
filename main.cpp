#include <QCoreApplication>
#include <cmath>
#include <istream>
#include <map>
#include "util.cpp"





int main(int argc, char* argv[])
{

    if(argc < 3)
    {
        cerr << "Quantidade de argumentos inválida!\n";
        exit(EXIT_FAILURE);
    }

    string path = argv[1];

    string image_codec = argv[3];

    if( image_codec.empty() )
    {
        cout << "Codec da base não informado!\n";
        exit(EXIT_FAILURE);
    }

    ifstream File;
    File.open(path+"/folders_name.txt");
    if(!File.is_open())
    {
        cerr << "Problema ao Abrir Nomes das Pastas" << '\n';
        exit(EXIT_FAILURE);
    }

    vector<string> folders_name;

    istream_iterator<string> eos;
    istream_iterator<string> input(File);
    copy_if(input,eos,std::back_inserter(folders_name),[](string a) { return a[0] != '#'; });


    int number_images = 0;


    image_base base{image_codec,path,folders_name.size(),number_images,
                    image_base::TYPE::BRODATZ,image_base::COLOR::GRAY};


    if(!base.create_arff_file(argv[2]))
    {
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
    }

    thread_handler(base,folders_name,false);


}
