#ifndef _PPM_HPP_
#define _PPM_HPP_

#include <string>

namespace PPM {

	// Read() creates memory dynamically - remember to delete[] it when you're finished using the image!
	// (You can delete the memory after creating the texture - the texture copies the data to graphics
	// memory.)
	
	unsigned char *Read(const std::string &filename, int &width, int &height);

	void Write(const std::string &filename, const unsigned char *img, const int width, const int height);

} // namespace PPM

#endif

