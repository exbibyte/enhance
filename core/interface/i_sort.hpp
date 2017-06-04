#ifndef I_SORT_HPP
#define I_SORT_HPP

#include <utility>
#include <functional>

namespace e2 { namespace interface {

template< class Impl >
class i_sort : public virtual Impl {
public:
	 template< class... Args >
         bool sort( Args ... args ){ return Impl::sort( std::forward<Args>( args )... ); }
};

} }
	
#endif
