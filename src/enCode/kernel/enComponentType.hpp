#ifndef EN_COMPONENT_TYPE_H
#define EN_COMPONENT_TYPE_H

//component types recognized by the core engine
enum class enComponentType {
    UNDEFINED,
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
