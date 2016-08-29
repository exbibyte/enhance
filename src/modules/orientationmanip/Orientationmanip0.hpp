#ifndef ORIENTATIONMANIP0_H
#define ORIENTATIONMANIP0_H

#include "IOrientationmanip.hpp"
#include "FilterUiDrag.hpp"
#include "Quat.h"

#include <list>

class Orientationmanip0 : public IOrientationmanip<Quat, drag_coordinate> {
public:
    char const * get_id(){ return "orientationmanip0"; }
    bool init() override;
    bool deinit() override;
    bool process( std::list<Quat> & out, std::list<drag_coordinate> & in ) override;
};

#endif
