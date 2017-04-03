#include "orientprop.hpp"

Quat & orientprop::get_rot_ref(){
    return orient.GetRealRef();
}
Quat & orientprop::get_trans_ref(){
    return orient.GetDualRef();
}
