#ifndef IQUEUE_H
#define IQUEUE_H

template< class T, template< class > class ContainerType >
class IQueue final : public ContainerType< T > {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = T const &;
    using size_type = typename ContainerType<T>::_t_size;

    bool clear(){ return ContainerType<T>::clear(); }
    bool empty(){ return ContainerType<T>::empty(); }
    size_type size(){ return ContainerType<T>::size(); }
    bool enqueue( value_type & item ){ return ContainerType<T>::enqueue( item ); }
    bool dequeue( value_type & item ){ return ContainerType<T>::dequeue( item ); }
    ~IQueue(){ this.~ContainerType<T>(); }
};

#endif
