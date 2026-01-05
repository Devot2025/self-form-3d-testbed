#include "std_file_manger.hpp"

void loading_all_file_data_binary(char *& dst_buffer, const char * file_path){
    if(!file_path) throw std::runtime_error("error : File path is empty.");
    std::ifstream read_file(file_path, std::ios::binary);
    size_t file_size = get_all_file_size(read_file);
    dst_buffer = new char[file_size + 1]{};
    if(!dst_buffer) throw std::runtime_error("error : Failed to memory allocated.");
    read_file.read(dst_buffer, file_size);
}