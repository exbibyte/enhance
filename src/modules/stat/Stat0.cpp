#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include "Stat0.hpp"
#include "IStat.hpp"

std::string const Stat0::getstat(){
    std::ifstream file( "/proc/self/status", std::ios::in );
    std::string statinfo;
    
    if( file.is_open() ){
	while( getline( file, statinfo ) ){
	    if ( std::string::npos != statinfo.find("VmRSS:") ){
		return statinfo;
	    }
	}
    }
    return "";
}
