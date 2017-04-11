#ifndef CAM0_HPP
#define CAM0_HPP

#include "ICam.hpp"
#include "IOrientationmanip.hpp"
#include "IUi.hpp"
#include "Quat.hpp"
#include "FilterUiDrag.hpp"

class cam0 : public ICam {
public:
    cam0();
    char const * get_id(){ return "cam0"; }
    bool init( e_pparam, void * ) override;
    bool deinit( e_pparam, void * ) override;
    bool process( e_pparam, void * ) override;

    bool _exit;
    double _pos[3];
    Quat _orientation;

private:
    bool process_operation( ICam::operation_mode * );
    IUi * _ui_register;
    IOrientationmanip<Quat, drag_coordinate> * _orientationmanip_register;
    FilterUiDrag * _filterdrag_register;
    bool _enabled;

};

#endif
