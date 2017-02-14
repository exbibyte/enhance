#ifndef DATASTORE0_H
#define DATASTORE0_H

#include "IDatastore.hpp"

class Datastore0 : public IDatastore {
public:
    Datastore0( size_t id_store ) : IDatastore( id_store ) {}
    char const * get_id(){ return "datastore0"; }
    bool fetch( size_t id, std::function< void(void* data, size_t len) > f ) override;
    bool store( size_t id, void* data, size_t len ) override;
};

#endif
