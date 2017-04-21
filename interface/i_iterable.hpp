#ifndef I_ITERABLE_HPP
#define I_ITERABLE_HPP

namespace i_iterable {
    template< class ContainerType, class IterableType >
    IterableType * i_begin( ContainerType * c, IterableType * tag ){ return nullptr; }
    
    template< class ContainerType, class IterableType >
    IterableType * i_end( ContainerType * c, IterableType * tag ){ return nullptr; }

    template< class ContainerType, class IterableType >
    IterableType * i_iterator_begin( ContainerType * c, IterableType * tag ){ return nullptr; }

    template< class ContainerType, class IterableType >
    IterableType * i_iterator_end( ContainerType * c, IterableType * tag ){ return nullptr; }

    template< class ContainerType, class IterableType >
    IterableType * i_prev( ContainerType * c, IterableType * i ){ return nullptr; }

    template< class ContainerType, class IterableType >
    IterableType * i_next( ContainerType * c, IterableType* i ){ return nullptr; }

    template< class ContainerType, class IterableType >
    IterableType * i_erase( ContainerType * c, IterableType * i ){ return nullptr; }

    template< class ContainerType, class IterableType >
    IterableType * i_advance( ContainerType * c, IterableType * i, long long n ){ return nullptr; }
}

#endif
