#include "file_md5_mesh.hpp"
#include <string>

int main( int argc, char** argv ){

    std::string path = argv[1];
    file_md5_mesh::data_wrap dw = file_md5_mesh::process( path );

    return 0;
}
