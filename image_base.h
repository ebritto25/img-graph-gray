#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using cv::Mat;
using namespace std;

class image_base
{
public:
    enum class TYPE {BRODATZ,RSSCN};
    enum class COLOR {GRAY, RGB};

    image_base(string codec,string path_folder,string path_to_image_, int folders, int images,TYPE type,COLOR scheme)
    : image_codec{codec} , path_to_folders{path_folder},path_to_image{path_to_image_} ,number_of_folders{folders},number_of_images{images}, base_type{type}, color_scheme{scheme}
    {
    }

    image_base() = default;
    ~image_base();

    bool create_arff_file(const string);
    void put_in_arff_file(const string);

    int folders() const;
    int images() const;

    void set_number_of_folders(const int);
    void set_number_of_images(const int);


    void set_path_to_folders(const string);
    string get_path_to_folders() const;

    void set_image_base_type(TYPE);
    TYPE get_image_base_type() const;


    void set_image_codec(const string);
    string get_image_codec() const;


    void set_base_color(COLOR scheme);
    COLOR color() const ;

    template<typename Folder>
    string get_image_in_folder(Folder,TYPE,int);


private:
    string image_codec;
    string path_to_folders;
    string path_to_image;
    int number_of_folders;
    int number_of_images;
    ofstream arff_file;
    COLOR color_scheme;
    Mat model_img;
    TYPE base_type;

};

#endif // IMAGE_BASE_H
