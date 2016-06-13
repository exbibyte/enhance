#ifndef EN_COMPONENT_TYPE_H
#define EN_COMPONENT_TYPE_H

//component types recognized by the core engine
enum class enComponentType {
    UNDEFINED,
    //FACTORY STARTS
    FACTORY,
    FACTORY_CLOCK,
    FACTORY_LOGGER,
    FACTORY_RENDER,
    FACTORY_UI,
    FACTORY_LOGIC,
    FACTORY_SCHEDULER,
    FACTORY_UTILITY,
    //FACTORY ENDS
    //INSTANCE STARTS
    CLOCK,
    LOGGER,
    RENDER,
    UI,
    LOGIC,
    SCHEDULER,
    UTILITY,
    //INSTANCE ENDS
};

#endif
