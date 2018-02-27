#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>


namespace bsf = boost::filesystem;

std::vector<std::string> get_classes_name(const bsf::path & base_path)
{
    if( !bsf::exists(base_path) )
    {
       std::cout << "Error getting to the base path!\n";
       return std::vector<std::string>();
    }

    bsf::directory_iterator end_itr;
    
    std::vector<std::string> classes_name;

    for(bsf::directory_iterator itr(base_path); itr != end_itr; itr++)
        if( is_directory( *itr ) ) 
             classes_name.push_back(itr->path().filename());
   
   return classes_name;
}
