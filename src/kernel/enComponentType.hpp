#ifndef EN_COMPONENT_TYPE_H
#define EN_COMPONENT_TYPE_H

//component types recognized by the core engine
enum class enComponentType {
    UNDEFINED,
    //INSTANCE STARTS
    CLOCK, 
    LOGGER,
    RENDERDRAW,
    RENDERCOMPUTE,
    RENDERSERVER,
    UI,
    FILTER,
    ORIENTATIONMANIP,
    CAM,
    LOGIC,
    SCHEDULER,
    WORKERPOOL,
    UTILITY,
    STAT,
    MEMORYALLOC,
    THREAD,
    INIT,
    PARSER,
    //INSTANCE ENDS
};

#endif
