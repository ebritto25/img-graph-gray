#include <QCoreApplication>
#include <iomanip>
#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <fstream>
#include <cmath>
#include "util.cpp"

std::mutex mt;

void extrai_valor(int j,int number_images,string image_codec,string path,string path_folder,ofstream& File,bool imagem_colorida)
{
    string ret{""};

    stringstream temp;
    stringstream temp_img;

    temp_img << path << "/" << to_string(j)<< "/" << char('a'+(j-1)) << setfill('0') << setw(3) << to_string(1) << image_codec;
    Mat image = imread(temp_img.str());

    std::cerr << temp_img.str() << '\n';
    if(imagem_colorida)
    {
        int from[] = {0,(image.cols-1),(image.cols-1 + image.cols*image.rows * 2),(image.cols*image.rows*2),
                    0,(image.cols-1),image.cols/3,image.cols*(image.rows/2),
                      image.cols*image.rows,(image.cols-1)+(image.cols*image.rows),(image.cols/2)+(image.cols*image.rows),image.cols*(image.rows/2)+(image.cols*image.rows),
                      image.cols*image.rows*2,(image.cols-1)+(image.cols*image.rows*2),(image.cols/2)+(image.cols*image.rows*2),image.cols*(image.rows/2)+(image.cols*image.rows*2)};

      igraph_vs_t to[16];

      define_pixels_destino(to,image,IMAGEM_COLORIDA);

      for(int i = 0; i < number_images; i++)
      {

          cout << "Imagem RSSCN:  " << i+1<< " de " << number_images << "  CLASSE: " << j << '\n';
          stringstream ss;

          char image_init = 'a' + (j-1);

          ss << path << "/" << to_string(j)<< "/" << image_init << setfill('0') << setw(3) << to_string(i+1) << image_codec;
          std::cout << ss.str()   << '\n';


          ret = atributeGenerator(ss.str(),to,from);
          ret += path_folder+to_string(j)+"\n";

          temp << ret;
      }

      mt.lock();
      File << temp.str();
      mt.unlock();

      for(int i = 0 ; i < 16; i++)
          igraph_vs_destroy(&to[i]);
    }
    else
    {
      int from[] = {0,(image.cols-1),image.cols/2,image.cols*(image.rows/2)};

      igraph_vs_t to[4];

      define_pixels_destino(to,image,IMAGEM_CINZA);

      for(int i = 0; i < number_images; i++)
      {

          cout << "Imagem RSSCN:  " << i+1<< " de " << number_images << "  CLASSE: " << j << '\n';
          stringstream ss;

          char image_init = 'a' + (j-1);

          ss << path << "/" << to_string(j)<< "/" << image_init << setfill('0') << setw(3) << to_string(i+1) << image_codec;
          std::cout << ss.str()   << '\n';


          ret = atributeGenerator(ss.str(),to,from);
          ret += path_folder+to_string(j)+"\n";

          temp << ret;
      }

      mt.lock();
      File << temp.str();
      mt.unlock();

      for(int i = 0 ; i < 4; i++)
          igraph_vs_destroy(&to[i]);
    }
}

enum class BASE { BRODATZ=0, RSSCN };


void base_imagem(BASE base_imagem,string  image_codec, string path_folder, string path, int numero_classes, int numero_imagens,bool imagem_colorida,ofstream& File)
{
        string str_out;
    std::vector<thread> threads;

    switch(base_imagem)
    {

            case BASE::BRODATZ :
        for(int j = 1; j <= numero_classes; j++)
        {

            for(int i = 0; i < numero_imagens/* 1024 terceira classe em diante*/; i++)
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
            if(j!=5)
                threads.emplace_back(extrai_valor,j,numero_imagens,image_codec,path,path_folder,std::ref(File),true);
        }

        for(int j = 1; j <= numero_classes; j++)
            threads[j].join();

        break;

      default:
    /*
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
        */
        break;
    }
}



int main(int argc, char* argv[])
{
    if(argc < 2)
        exit(-1);


    // MUDAR DE ACORDO COM A BASE DE IMAGENS


    int number_classes = 7, number_images = 400;

    string path = argv[1],str_out;


    ofstream File;
    string output_arff_name = argv[2];


    File.open(path+output_arff_name);





    string path_folder = "sub_class";
    string image_codec = ".jpg";


    base_imagem(BASE::RSSCN,image_codec,path_folder,path,number_classes,number_images,true,File);

    if(!File.is_open())
     {
            cerr << "PROBLEMA AO ABRIR ARQUIVO ARFF\n";
            exit(-2);
    }



    File.close();

}
