#ifndef EN_COMPONENT_STAT_H
#define EN_COMPONENT_STAT_H

#include "enComponentSpecialize.hpp"
#include "IStat.hpp"
#include "Stat0.hpp"

class enComponentStat0 : public enComponentSpecialize<IStat, Stat0> {
public:
    enComponentStat0( Stat0 * instance ) : enComponentSpecialize<IStat, Stat0>( instance, enComponentType::STAT ){}
};

#endif
