#ifndef E2_THREAD_MANAGER_HPP
#define E2_THREAD_MANAGER_HPP

#include <vector>

template < class TypeThread >
class thread_manager {
public:
    std::vector< TypeThread * > _threads;
};

#endif
