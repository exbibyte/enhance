#ifndef UI0_H
#define UI0_H

#include "IUi.hpp"

#include "GLIncludes.hpp"

#include <set>
#include <list>
#include <unordered_map>

class Uitrackball0 : public IUi {
public:
    char const * get_id(){ return "ui0"; }
    bool get_characters( std::list<character> & );
    bool init();
    bool deinit();
};

#endif
