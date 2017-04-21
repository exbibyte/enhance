#ifndef I_LIST_HPP
#define I_LIST_HPP

namespace i_list {
    template< class ContainerType >
               bool i_list_clear( ContainerType * c ){ return false; }

    template< class ContainerType >
             size_t i_list_size( ContainerType * c ){ return 0; }

    template< class ContainerType >
             size_t i_list_update_size( ContainerType * c ){ return 0; }

    template< class ContainerType, class ValType >
               bool i_list_push_back( ContainerType * c, ValType const * item ){ return false; }

    template< class ContainerType, class ValType >
               bool i_list_push_front( ContainerType * c, ValType const * item ){ return false; }

    template< class ContainerType, class ValType >
               bool i_list_pop_back( ContainerType * c, ValType * item ){ return false; }

    template< class ContainerType, class ValType >
               bool i_list_pop_front( ContainerType * c, ValType * item ){ return false; }

    template< class ContainerType, class ValType >
               bool i_list_front( ContainerType * c, ValType * item ){ return false; }

    template< class ContainerType, class ValType >
               bool i_list_back( ContainerType * c, ValType * item ){ return false; }
}

namespace i_spliceable {
    template< class ContainerType, class IterableType >
    bool i_list_splice_entire( ContainerType * to_list, IterableType * insert_at, ContainerType * from_list );
    
    template< class ContainerType, class IterableType >
    bool i_list_splice_single( ContainerType * to_list, IterableType * insert_at, ContainerType * from_list, IterableType * insert_from );
    
    template< class ContainerType, class IterableType >
    bool i_list_splice_range( ContainerType * to_list, IterableType * insert_at, ContainerType * from_list, IterableType * insert_from_iter_begin, IterableType * insert_from_iter_end );
}

#endif
