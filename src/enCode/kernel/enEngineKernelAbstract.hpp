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
    ~enEngineKernelAbstract(){}
    bool RegisterComponent( enComponentMeta * component, uint_fast32_t cid = 0 );
    enComponentMeta * GetComponent( enComponentType type, uint_fast32_t cid = 0 );
    bool RemoveComponentAll();
    enEngineKernelAbstract * GetCoreEngine();
    void AccumulateComponents( std::function<void(enComponentMeta*,std::vector<enComponentMeta*>&)> f, std::vector<enComponentMeta * > & accum );
    std::map< std::pair<enComponentType,uint_fast32_t>, enComponentMeta * > _components;
    uint_fast32_t _cid = 0; //component id

    //stages for custom actions starts-----
    virtual void Init(){}
    virtual void Deinit(){}
    //stages for custom actions ends-----

    //API section starts-----
    void transmit( enPacketTransmit packet ){}
    //API section ends-----
};

#endif
