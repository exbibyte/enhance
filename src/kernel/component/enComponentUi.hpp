#ifndef EN_COMPONENT_UI_H
#define EN_COMPONENT_UI_H

#include <chrono>

#include "enComponentSpecialize.hpp"
#include "IUi.hpp"
#include "Ui0.hpp"

class enComponentUi0 : public enComponentSpecialize<IUi, Ui0> {
public:
    enComponentUi0( Ui0 * instance ) : enComponentSpecialize<IUi, Ui0>( instance, enComponentType::UI ){}
};

#endif
