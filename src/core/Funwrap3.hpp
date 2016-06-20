#ifndef FUNCWRAP3_H
#define FUNCWRAP3_H

#include <thread>
#include <functional>
#include <type_traits>

enum class FunCallType {
    SYNC,
    ASYNC,	
};

class RetVoid {};
class RetNonVoid{};

class Funwrap3 {
public:
    Funwrap3() : _fun(nullptr), _calltype(FunCallType::ASYNC) {}
    template< class F, class... Args >
    void set( FunCallType calltype, F f, Args... args ){
	using ResultType = typename std::result_of<F(Args...) >::type;
	using tag_dispatch_type = typename std::conditional<std::is_void<ResultType>::value, RetVoid, RetNonVoid>::type;
	tag_dispatch_type tag;
	set_aux( tag, calltype, f, args... );
    }
    template< class F, class... Args >
    void set_aux( RetNonVoid tag, FunCallType calltype, F f, Args... args ){
	_calltype = calltype;
	using ResultType = typename std::result_of<F(Args...) >::type;
	ResultType _result;
	_fun = std::function<void()>([=]()->void {
		ResultType result = f( args... );
		return;
	    });
    }
    template< class F, class... Args >
    void set_aux( RetVoid tag, FunCallType calltype, F f, Args... args ){
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
    FunCallType _calltype;
    
};

#endif
