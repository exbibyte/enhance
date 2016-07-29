#ifndef ALLOC_H
#define ALLOC_H

template< typename policy >
class alloc {
public:
    using type = policy;
};

#endif
