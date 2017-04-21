#ifndef I_BASIC_HPP
#define I_BASIC_HPP

namespace i_startup {
    template< class Type, class... Args >
    bool i_init( Type * c, Args && ... args ){ return false; }

    template< class Type, class... Args >
    bool i_deinit( Type * c, Args && ... args ){ return false; }
};

#endif
