#include <QCoreApplication>
#include <cmath>
#include <istream>
#include "util.cpp"


/*
      default:
        for(int j = 1;j <= 10;j++)
        {
                int mult = (j-1)*100;
                for(int i = mult+0;i < mult+100;i++)
                {
                    std::cout << "Imagem:  " << i+1<< " de 1000.\n";

                    if(!imagem_colorida)
                        str_out = atributeGenerator_gray(path+"/"+path_folder+to_string(j)+"/"+to_string(i)+image_codec)+path_folder+to_string(j)+"\n";
                    else

                        //str_out = atributeGenerator(path+"/"+path_folder+to_string(j)+"/"+to_string(i)+image_codec)+path_folder+to_string(j)+"\n";

                    cerr << "ARFF OUTPUT: " << str_out << '\n';
                    File << str_out;
                }
        }
        break;
    }
}
*/

int main(int argc, char* argv[])
{
    if(argc < 2)
        exit(-1);


    // Caso especial base UCMERCED
        ifstream File;
        File.open("/media/rodrigo/A96C-4A45/Kylberg/folders_name.txt");
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
