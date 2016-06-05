#ifndef EN_COMPONENT_TYPE_H
#define EN_COMPONENT_TYPE_H

//component types recognized by the core engine
enum class enComponentType {
    UNDEFINED,
    LOGGER,
    CORE_UTILITY,
    CORE_SCHEDULE,
    LOGIC_FRONTEND,
    LOGIC_BACKEND,
    UI,
    RENDER,
    CLOCK,
    TRACKER_SYS_RESOURCE,
    TRACKER_COMPONENTS,
};

#endif
