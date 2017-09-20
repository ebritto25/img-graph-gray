#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <cmath>
#include "util.cpp"


int main(int argc, char* argv[])
{
    if(argc != 2)
        exit(-1);

    bool gray_img = true, brodatz = true;

    string path = argv[1],str_out;

    ofstream File;
    string output_arff_name = "/output.arff";

    File.open(path+output_arff_name);



    string path_folder = "sub_class_";
    string image_codec = ".tiff";




    if(!File.is_open())
     {
            cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
            exit(-2);
    }

    if(!gray_img)
    {
            for(int j = 1;j <= 10;j++)
            {
                    int mult = (j-1)*100;
                    for(int i = mult+0;i < mult+100;i++)
                    {
                         std::cout << "Imagem:  " << i+1<< " de 1000.\n";
                        str_out = atributeGenerator(path+"/"+path_folder+to_string(j)+"/"+to_string(i)+image_codec)+path_folder+to_string(j)+"\n";
                        File << str_out;
                    }
            }
    }
    else
    {
        if(!brodatz)
        {

            for(int j = 1;j <= 10;j++)
            {
                    int mult = (j-1)*100;
                    for(int i = mult+0;i < mult+100;i++)
                    {
                         std::cout << "Imagem:  " << i+1<< " de 1000.\n";
                        str_out = atributeGenerator_gray(path+"/"+path_folder+to_string(j)+"/"+to_string(i)+image_codec)+path_folder+to_string(j)+"\n";
                        cerr << "ARFF OUTPUT: " << str_out << '\n';
                        File << str_out;
                    }
            }
         }
         else
        {
            for(int j = 1; j <= 13; j++)
            {

                for(int i = 0; i < 256/* 1024 terceira classe em diante*/; i++)
                {

                        std::cout << "Imagem BROADTZ:  " << i+1<< " de 256 CLASSE: " << j << '\n';
                        str_out = atributeGenerator_gray(path+"/"+path_folder+to_string(j)+"/"+"output"+to_string(j)+"_"+to_string(i)+image_codec);
                        str_out += path_folder+to_string(j)+"\n";

                        File << str_out;
                }


            }



        }

    }

    File.close();

    return 0;
}
