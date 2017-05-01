#ifndef E2_I_STAT_HPP
#define E2_I_STAT_HPP

namespace e2 { namespace traits {

enum class e_stat_type {
    stat_time_fraction,
};

} }
namespace e2 { namespace interface {

template< Impl >
class i_stat {
public:
    template< class ... Args >
    i_stat( Args && ... args ) : Impl( std::move( args )... ) {}
    template< class ... Args >
    bool stat_process( Args ... args ){ Impl::stat_process( args ... ); }

    ::e2::traits::e_stat_type _stat_type;
};

} }

#endif
