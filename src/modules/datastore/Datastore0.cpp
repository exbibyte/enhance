#include <functional>
#include <cstring>

#include "Datastore0.hpp"

using namespace std;

bool Datastore0::fetch( size_t id, std::function< void(void* data, size_t len) > f ){
    return false;
}
bool Datastore0::store( size_t id, void* data, size_t len ){
    return false;
}
