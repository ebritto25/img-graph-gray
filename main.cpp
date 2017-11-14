#include <QCoreApplication>
#include <cmath>
#include <istream>
#include <map>
#include "util.cpp"





int main(int argc, char* argv[])
{

    if(argc < 6)
    {
        cerr << "Quantidade de argumentos inválida!\n";
        exit(EXIT_FAILURE);
    }

    map<string,int> bases;

    bases["brodatz"] = 1;
    bases["rgbbrodatz"] = 2;
    bases["ucm"] = 3;
    bases["rsscn"] = 4;
    bases["kylberg"] = 5;

    string path = argv[1];
    string base_name = argv[2];
    int number_of_images = argv[3];
    string image_codec = argv[4];
    bool mst = argv[5];
    string arff_file = argv[6]; 




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

    int number_of_folders = folders_name.size(); 

    image_base::TYPE base_type;
    if(bases[base_name] ==  bases["brodatz"])
        base_type = image_base::TYPE::BRODATZ;
    else if(bases[base_name] ==  bases["rgbbrodatz"])
        base_type = image_base::TYPE::RGBBRODATZ;
    else if(bases[base_name] == bases["ucm"])
        base_type = image_base::TYPE::UCM;
    else if(bases[base_name] == bases["rsscn"])
        base_type = image_base::TYPE::RSSCN;
    else if(bases[base_name] == bases["kylberg"])
        base_type = image_base::TYPE::KYLBERG;

    image_base base{image_codec,path,number_of_folders,number_of_images,
                        base_type,image_base::COLOR::GRAY};


    if(!base.create_arff_file(arff_file))
    {
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
    }


    thread_handler(base,folders_name,mst);


}
