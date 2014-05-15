#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PPM.hpp"

namespace PPM {

unsigned char *Read(const std::string &filename, int &width, int &height)
{
	FILE *ifp;
	char buffer[80];
	int i;
	int header[3]; // width, height, maxval
	int tmp;

	// Warning - Number of references to this class might cause problems when reading images
	  
	ifp = fopen(filename.c_str(), "rb");
	if( !ifp ) {
		throw std::string("Error opening file \"") + filename + std::string("\"");
	}
	  
	i = 0;
	fgets(buffer, 80, ifp);
	if( strncmp(buffer, "P6", 2) ) {
		fclose(ifp);
		throw std::string("File is not in valid PPM format");
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
	unsigned char *img = new unsigned char[width*height*3];

	fread(img, 3, width * height, ifp);
	  
	fclose(ifp);
	return img;
}

void Write(const std::string &filename, const unsigned char *img, const int width, const int height)
{
	FILE *ifp;
	ifp = fopen(filename.c_str(), "wb");
	if( !ifp ) {
		throw std::string("Error opening file \"") + filename + std::string("\"");
	}
	  
	fprintf(ifp, "P6\n%d %d\n255\n", width, height);

	fwrite(img, 3, width * height, ifp);
	  
	fclose(ifp);
}

} // namespace PPM

