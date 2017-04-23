#ifndef I_ITERABLE_HPP
#define I_ITERABLE_HPP

template< class IterableType >
class i_iterable {
    IterableType * begin(){ return nullptr; }
    IterableType * end(){ return nullptr; }
    IterableType * iterator_begin(){ return nullptr; }
    IterableType * iterator_end(){ return nullptr; }
    IterableType * prev( IterableType * i ){ return nullptr; }
    IterableType * next( IterableType* i ){ return nullptr; }
    IterableType * erase( IterableType * i ){ return nullptr; }
    IterableType * advance( IterableType * i, long n ){ return nullptr; }
};

#endif
