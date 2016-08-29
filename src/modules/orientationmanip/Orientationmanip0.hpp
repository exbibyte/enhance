#ifndef ORIENTATIONMANIP0_H
#define ORIENTATIONMANIP0_H

#include "IOrientationmanip.hpp"
#include "FilterUiDrag.hpp"
#include "Quat.h"

#include <list>
#include <initializer_list>

class Orientationmanip0 : public IOrientationmanip<Quat, drag_coordinate> {
public:
    Orientationmanip0() : _w(500), _h(500), _start_x(250), _start_y(250) {}
    char const * get_id(){ return "orientationmanip0"; }
    bool init( std::initializer_list<int> const & args ) override;
    bool deinit() override;
    bool process( std::list<Quat> & out, std::list<drag_coordinate> & in ) override;
private:
    int _w;
    int _h;
    int _start_x;
    int _start_y;
};

#endif
