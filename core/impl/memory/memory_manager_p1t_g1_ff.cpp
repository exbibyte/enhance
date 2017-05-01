#include "memory_manager_p1t_g1_ff.hpp"

namespace e2 { namespace memory {

memory_manager_p1t_g1_ff_impl::memory_manager_p1t_g1_ff_impl(){
    size_t s = (size_t) 1 << 16;
    _partition.internal_resize( s ); //65kB
}
memory_manager_p1t_g1_ff_impl::~memory_manager_p1t_g1_ff_impl(){

}

} }
