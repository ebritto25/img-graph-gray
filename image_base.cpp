#include <fstream> 

enum class COLOR{ GRAY, RGB};

struct image_base
{
    std::string path;
    COLOR image_color;
    std::ofstream arff_file;
};
