#ifndef EN_ENGINE_KERNEL_ABSTRACT_H
#define EN_ENGINE_KERNEL_ABSTRACT_H

#include "enComponentType.hpp"
#include "enPacketTransmit.hpp"
#include "enComponentMeta.hpp"

#include <map>
#include <functional>
#include <cstdint>
#include <vector>

//interface for core engine
class enEngineKernelAbstract {
public:
    enEngineKernelAbstract(){}
    virtual ~enEngineKernelAbstract(){}

    //API section starts-----
    bool register_component( enComponentMeta * component );
    enComponentMeta * get_component( enComponentType type, uint_fast32_t cid = 0 );
    enEngineKernelAbstract * get_core_engine();
    void accumulate_components( std::function<bool(enComponentMeta*)> f, std::vector<enComponentMeta * > & accum );
    void get_components_by_type( enComponentType type, std::vector< enComponentMeta * > & accum );
    void transmit( enPacketTransmit packet ){}
    int get_num_components() const;
    //API section ends-----
    
protected:
    //stages for custom actions starts-----
    virtual void init(){}
    virtual void deinit(){}
    //stages for custom actions ends-----

    bool remove_component_all();
    
private:
    std::map< std::pair<enComponentType,uint_fast32_t>, enComponentMeta * > _components;
    uint_fast32_t _cid = 0; //component id
};

#endif
