#ifndef IDATASTORE_H
#define IDATASTORE_H

#include <cstring>
#include <functional>

class IDatastore {
public:
    size_t _id_store;
    IDatastore( size_t id_store ) : _id_store(id_store) {}
    virtual ~IDatastore(){}
    virtual bool fetch( size_t id, std::function< void(void* data, size_t len) > f ) = 0;
    virtual bool store( size_t id, void* data, size_t len ) = 0;
};

#endif
