#include <QCoreApplication>
#include <iostream>
#include <cmath>
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



    int number_folders = 13, number_images = 256;

    string path = argv[1],str_out;
    string image_codec = ".tiff";


    image_base base{image_codec,path,number_folders,number_images,
                image_base::TYPE::BRODATZ,image_base::COLOR::GRAY};

    if(!base.create_arff_file(argv[2]))
    {
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
    }

    thread_handler(base);

}
