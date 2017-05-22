#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstring>

#include "text_read_write.hpp"

namespace e2 { namespace file {

bool text_read_write::read(char const * fn, std::vector<char> * read ){
    FILE *fp;
    int count=0;

    if( nullptr == read ) return false;
    if( nullptr == fn ) return false;

    fp = fopen(fn,"rt");
    if (nullptr == fp) return false;
    
    fseek(fp, 0, SEEK_END);
    count = ftell(fp);
    rewind(fp);

    std::vector<char> text;
    if (count > 0) {
        text.resize( count + 1 );
	count = fread( &text[0], sizeof(char), count, fp);
        text[count] = '\0';
    }
    *read = text;
    fclose(fp);

    return true;
}

bool text_read_write::write(char const * fn, char const * s ){
    FILE *fp;
    bool ret;
    
    if( nullptr == fn ) return false;
    fp = fopen(fn,"w");

    if( nullptr == fp ) return false;
			
    if( fwrite( s, sizeof(char), strlen(s), fp ) == strlen( s ) )
	ret = true;
    else
	ret = false;

    fclose(fp);

    return ret;
}

} }
