#include <fstream>
#include <vector>
#include <iostream>

enum class COLOR{ GRAY, RGB};

struct image_base
{
    void init_arff_file(bool with_mst,bool rgb)
    {
        arff_file << "@RELATION CLASSES\n";

        int att_lines = 8;

        if(rgb)
        {
            att_lines *= 4;
            att_lines += 6;
        }
        if(with_mst)
            att_lines += 2;


        for(int i = 0; i < att_lines ;i++)
            arff_file << "@ATTRIBUTE atb" << i << " NUMERIC\n";
    }

    void put_classes_in_arff(std::vector<std::string> classes_name)
    {
        arff_file << "@ATTRIBUTE class_ { ";

        for(int i = 0; i < classes_name.size(); i++)
        {
           if( (i+1) < classes_name.size())
               arff_file << "class_" << classes_name[i] << ',';
           else
               arff_file << "class_" << classes_name[i] << "}\n";
        }

        arff_file << "@DATA\n";
    }

    std::string path;
    COLOR image_color;
    std::ofstream arff_file;
};
