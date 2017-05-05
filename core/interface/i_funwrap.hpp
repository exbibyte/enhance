#ifndef E2_I_FUNWRAP_HPP
#define E2_I_FUNWRAP_HPP

#include <thread>
#include <functional>
#include <type_traits>

namespace e2 { namespace interface {

enum class i_funtype {
    SYNC,
    ASYNC,	
};

class i_funwrap {
private:
    class RetVoid {};
    class RetNonVoid{};
public:
    i_funwrap() : _fun(nullptr), _calltype(i_funtype::ASYNC) {}
    template< class F, class... Args >
    void set( i_funtype calltype, F f, Args... args ){
	using ResultType = typename std::result_of<F(Args...) >::type;
	using tag_dispatch_type = typename std::conditional<std::is_void<ResultType>::value, i_funwrap::RetVoid, i_funwrap::RetNonVoid>::type;
	tag_dispatch_type tag;
	set_aux( tag, calltype, f, args... );
    }
    template< class F, class... Args >
    void set_aux( i_funwrap::RetNonVoid tag, i_funtype calltype, F f, Args... args ){
	_calltype = calltype;
	using ResultType = typename std::result_of<F(Args...) >::type;
	ResultType _result;
	_fun = std::function<void()>([=]()->void {
		ResultType result = f( args... );
		return;
	    });
    }
    template< class F, class... Args >
    void set_aux( i_funwrap::RetVoid tag, i_funtype calltype, F f, Args... args ){
	_calltype = calltype;
	using ResultType = typename std::result_of<F(Args...) >::type;
	_fun = std::function<void()>([=]()->void {
		f( args... );
		return;
	    });
    }	
    void apply(){
	if( nullptr != _fun ){
	    _fun();
	}
	_fun = nullptr;
    }
private:
    std::function<void()> _fun;
    i_funtype _calltype;
};

} }

#endif
