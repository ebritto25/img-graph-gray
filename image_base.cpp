#include "image_base.h"


image_base::~image_base()
{
    arff_file.close();
}

bool image_base::create_arff_file(const std::string file_path)
{
    arff_file.open(file_path);

    return arff_file.is_open();
}

void image_base::put_in_arff_file(const std::string line)
{
    arff_file << line;
}


int image_base::folders() const  { return number_of_folders; }
int image_base::images() const  { return number_of_images; }

void image_base::set_number_of_folders(const int folders)
{
    number_of_folders = folders;
}

void image_base::set_number_of_images(const int images)
{
    number_of_images = images;
}

void image_base::set_path_to_folders(const std::string path)
{
    path_to_folders = path;
}

std::string image_base::get_path_to_folders() const
{
    return path_to_folders;
}

void image_base::set_image_base_type(TYPE base_type)
{
    this->base_type = base_type;
}

image_base::TYPE image_base::get_image_base_type() const
{
    return base_type;
}


void image_base::set_image_codec(const std::string image_codec)
{
    this->image_codec = image_codec;
}

std::string image_base::get_image_codec() const
{
    return image_codec;
}

void image_base::set_base_color(COLOR scheme)
{
    color_scheme = scheme;
}

image_base::COLOR image_base::color() const
{
    return color_scheme;
}

template<>
string image_base::get_image_in_folder(int folder,image_base::TYPE type,int img_number)
{
    std::stringstream ret;
    if(type == image_base::TYPE::RSSCN)
        ret << path_to_folders << "/" << folder << "/" << char('a'+(folder-1)) << setfill('0') << setw(3) << img_number+1 << image_codec;
    else if(type == image_base::TYPE::BRODATZ)
        ret << path_to_folders << "/" << folder << "/output" << folder  << "_" << img_number << image_codec;


    return ret.str();
}

template<> // Base UCM
string image_base::get_image_in_folder(string folder,image_base::TYPE type,int img_number)
{
    std::stringstream ret;

    ret << path_to_folders << "/" << folder << "/" << folder << setfill('0') << setw(2) << img_number  << image_codec;


    return ret.str();
}



