#include <string>
#include <cstdlib>
#include <cstring>

#include "Stat0.hpp"
#include "IStat.hpp"

std::string const Stat0::getstat(){
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    std::string statinfo;
    while (fgets(line, 128, file) != NULL){
	if (strncmp(line, "VmRSS:", 6) == 0){
	    statinfo = line;
	    break;
	}
    }
    fclose(file);
    return statinfo;
}
