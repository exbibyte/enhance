#ifndef EN_COMPONENT_META
#define EN_COMPONENT_META

class enComponentMeta {
public:
    enComponentMeta( enComponentType type ) : _component_type(type) {}
    void SaveKernelInfo( enEngineKernelAbstract * kernel, uint_fast32_t id ){
        _kernel = kernel;
        _id = id;
    }
    enComponentType _component_type;
private:
    uint_fast32_t _id;
    enEngineKernelAbstract * _kernel;
    void send( enComponentType dest, std::vector<uint_fast32_t> msg ){
        send( dest, 0, msg );
    }
    void send( enComponentType dest, uint_fast32_t dest_id, std::vector<uint_fast32_t> msg ){
        _kernel->send( _component_type, _id, dest, dest_id, msg ); 
    }
};

#endif
