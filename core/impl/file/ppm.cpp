#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <cstring>

#include "ppm.hpp"

namespace e2 { namespace file {

bool ppm::read( char const * filename, int &width, int &height, std::vector<unsigned char> * data )
{
    if( nullptr == data ) return false;
    FILE *ifp;
    char buffer[80];
    int i;
    int header[3]; // width, height, maxval
    int tmp;

    // Warning - Number of references to this class might cause problems when reading images
	  
    ifp = fopen( filename, "rb" );
    if( !ifp ) {
	assert( false );
	return false;
    }
	  
    i = 0;
    fgets(buffer, 80, ifp);
    if( strncmp(buffer, "P6", 2) ) {
	fclose(ifp);
	assert( false && "invalid PPM format" );
	return false;
    }

    while( (i < 3) ) {
	if( (tmp=fgetc(ifp)) == '#' ) {
	    fgets(buffer, 80, ifp); // read out comment
	    continue;
	} else {
	    ungetc(tmp, ifp);
	    fscanf(ifp, "%d", &header[i++]);
	}
    }
    fgets(buffer, 80, ifp); // read to newline

    // Renew image
    width = header[0];
    height = header[1];
    data->resize( width*height*3 );

    fread( &(*data)[0], 3, width * height, ifp);

    fclose(ifp);

    return true;
}

bool ppm::write( char const * filename, unsigned char const * data, const int width, const int height)
{
	FILE *ifp;
	ifp = fopen( filename, "wb" );
	if( !ifp ) {
	    assert( false && "opening file unsuccessful" );
	    return false;
	}
	  
	fprintf(ifp, "P6\n%d %d\n255\n", width, height);

	fwrite( data, 3, width * height, ifp );
	  
	fclose(ifp);
}

} }
