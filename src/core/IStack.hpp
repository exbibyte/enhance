#ifndef ISTACK_H
#define ISTACK_H

template< class T, template< class > class ContainerType >
class IStack final : public ContainerType< T > {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = T const &;
    using size_type = typename ContainerType<T>::_t_size;

    bool empty(){ return ContainerType<T>::empty(); }
    size_type size(){ return ContainerType<T>::size(); }
    bool push( value_type & item ){ return ContainerType<T>::push( item ); }
    bool pop( value_type & item ){ return ContainerType<T>::pop( item ); }
    ~IStack(){ this->~ContainerType<T>(); }
};

#endif
