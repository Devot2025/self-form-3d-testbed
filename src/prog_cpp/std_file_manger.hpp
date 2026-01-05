#ifndef _STD_FILE_MANAGER_HPP_
#define _STD_FILE_MANAGER_HPP_
#include <cpp_standard_lib.hpp>
inline size_t get_all_file_size(std::ifstream & read_file){
    read_file.seekg(0, std::ios::end);
    size_t file_size = read_file.tellg();
    read_file.seekg(0, std::ios::beg);
    return file_size;
}
void loading_all_file_data_binary(char *& dst_buffer, const char * file_path);
#endif