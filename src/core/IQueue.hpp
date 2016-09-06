#ifndef IQUEUE_H
#define IQUEUE_H

template< class T, template< class > class ContainerType >
class IQueue : public ContainerType< T > {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = T const &;
    using size_type = typename ContainerType<T>::_t_size;

    size_type size(){ return ContainerType<T>::size(); }
    bool enqueue( value_type & item ){ return ContainerType<T>::enqueue( item ); }
    bool dequeue( value_type & item ){ return ContainerType<T>::dequeue( item ); }
    virtual ~IQueue(){}
};

#endif
