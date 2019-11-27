#pragma once

#include <boost/filesystem.hpp>
#include <iostream> // REMOVER LINHAS COM "DEBUG"
#include <vector>

namespace bsf = boost::filesystem;

std::string get_base_name(const bsf::path & base_path);
std::vector<std::string> get_classes_name(const bsf::path & base_path);
std::vector<std::string> get_images_in_class(const bsf::path & class_path);

