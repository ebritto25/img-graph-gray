#include <QCoreApplication>
#include <iostream>
#include <cmath>
#include "util.cpp"



enum class BASE { BRODATZ, RSSCN };
/*
void base_imagem(BASE base_imagem,string  image_codec, string path_folder, string path, int numero_classes, int numero_imagens,bool imagem_colorida,ofstream& File)
{
        string str_out;
    std::vector<thread> threads;

    switch(base_imagem)
    {

            case BASE::BRODATZ :
        for(int j = 1; j <= numero_classes; j++)
        {

            for(int i = 0; i < numero_imagens; i++)
            {
                    std::cout << "Imagem BROADTZ:  " << i+1<< " de 256 CLASSE: " << j << '\n';
                        str_out = atributeGenerator_gray(path+"/"+path_folder+to_string(j)+"/"+"output"+to_string(j)+"_"+to_string(i)+image_codec);
                    str_out += path_folder+to_string(j)+"\n";
                    File << str_out;
            }
        }
        break;

      case BASE::RSSCN:


        for(int j = 1; j <= numero_classes; j++)
        {
          //  if(j!=5)
                //threads.emplace_back(extrai_valor,j,numero_imagens,image_codec,path,path_folder,std::ref(File),true);
        }

        for(int j = 1; j <= numero_classes; j++)
            //threads[j].join();

        break;

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



    int number_folders = 7, number_images = 400;
    string path = argv[1],str_out;
    string path_to_images = "sub_class";
    string image_codec = ".jpg";


    image_base base{image_codec,path,path_to_images,number_folders,number_images,
                image_base::TYPE::RSSCN,image_base::COLOR::RGB};

    if(!base.create_arff_file(argv[2]))
    {
        cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
        exit(EXIT_FAILURE);
    }

    extrai_valor(1,base);


}
