#ifndef I_STARTUP_HPP
#define I_STARTUP_HPP

class i_startup {
public:
    template< class Type, class... Args >
    bool i_init( Type * c, Args && ... args ){ return false; }

    template< class Type, class... Args >
    bool i_deinit( Type * c, Args && ... args ){ return false; }
};

#endif
