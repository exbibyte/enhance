#ifndef EN_COMPONENT_FILTER_H
#define EN_COMPONENT_FILTER_H

#include <chrono>

#include "enComponentSpecialize.hpp"
#include "IFilter.hpp"
#include "FilterUiDrag.hpp"

class enComponentFilterUiDrag : public enComponentSpecialize<IFilter<drag_coordinate,IUi::character>, FilterUiDrag> {
public:
    enComponentFilterUiDrag( FilterUiDrag * instance ) : enComponentSpecialize<IFilter<drag_coordinate,IUi::character>, FilterUiDrag>( instance, enComponentType::FILTER ){}
};

#endif
