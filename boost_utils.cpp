#include <boost/filesystem.hpp>
#include <iostream> // REMOVER LINHAS COM "DEBUG"
#include <vector>


namespace bsf = boost::filesystem;

std::string get_base_naem(const bsf::path & base_path) { return base_path.filename().string(); }

std::vector<std::string> get_classes_name(const bsf::path & base_path)
{
     if( !bsf::exists(base_path) )
    {
       std::cout << "Error getting to the base path!\n";
       return {};
    }

    bsf::directory_iterator end_itr;
    
    std::vector<std::string> classes_name;

    for(bsf::directory_iterator itr(base_path); itr != end_itr; itr++)
        if( is_directory( itr->status()) )
            classes_name.emplace_back( itr->path().filename().c_str());

   
   return classes_name;
}


std::vector<std::string> get_images_in_class(const bsf::path & class_path)
{    

    if( !bsf::exists(class_path) )
    {
       std::cout << "Error getting to the images path!\n";
       return {};
    }
    
    bsf::directory_iterator end_itr;

    std::vector<std::string> images_path;

    for(bsf::directory_iterator file(class_path); file != end_itr; file++)
        if( bsf::is_regular_file(*file) )
            images_path.emplace_back( file->path().c_str() );
    


    return images_path;
}
