#ifndef IPOOL_H
#define IPOOL_H

#include <utility>

class trait_pool_size {
public:
    class bounded{};
    class unbounded{};
};

class trait_pool_method {
public:
    class total{};
    class partial{};
    class synchronous{};
};

class trait_pool_fairness {
    class fifo{};
    class lifo{};
};

template< class T, template< class > class ContainerType, class PoolSize, class PoolMethod, class PoolFairness >
class IPool final : public ContainerType<T> {
public:
    //container and value traits
    using container_type = ContainerType<T>;
    using value_type = T;
    using reference = T &;
    using const_reference = T const &;
    using size_type = typename ContainerType<T>::_t_size;

    //pool traits
    using pool_size = PoolSize;
    using pool_method = PoolMethod;
    using pool_fairness = PoolFairness;

    template< class... Args >
    IPool( Args... args ) : ContainerType<T>( std::forward<Args>(args)... ) {}
    ~IPool(){ this->~ContainerType<T>(); }
    
    bool clear(){ return ContainerType<T>::clear(); }
    bool empty(){ return ContainerType<T>::empty(); }
    size_type size(){ return ContainerType<T>::size(); }
    bool put( value_type const & item ){ return ContainerType<T>::put( item ); }
    bool get( value_type & item ){ return ContainerType<T>::get( item ); }
};

#endif
