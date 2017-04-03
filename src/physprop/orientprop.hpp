#ifndef ORIENTPROP_HPP
#define ORIENTPROP_HPP

#include "DualQuat.hpp"
#include "Quat.hpp"

class orientprop {
public:
    Quat & get_rot_ref();
    Quat & get_trans_ref();
    DualQuat _orient;
};

#endif
