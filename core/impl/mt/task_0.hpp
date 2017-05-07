#ifndef E2_TASK_0_HPP
#define E2_TASK_0_HPP

#include "i_task.hpp"

namespace e2 { namespace mt {

class task_0_impl {
private:
    class RetVoid {};
    class RetNonVoid{};
public:
    task_0_impl() : _fun( nullptr ), _ret_addr( nullptr ) {}
    template< class F, class... Args >
    void set( void * ret_addr, F f, Args... args ){
	using ResultType = typename std::result_of< F( Args... ) >::type;
	using tag_dispatch_type = typename std::conditional<std::is_void<ResultType>::value, task_0_impl::RetVoid, task_0_impl::RetNonVoid>::type;
	tag_dispatch_type tag;
	set_aux( tag, ret_addr, f, args... );
    }
    inline void apply(){
	if( nullptr != _fun ){
	    _fun();
	}
	_fun = nullptr;
    }
private:
    template< class F, class... Args >
    inline void set_aux( task_0_impl::RetNonVoid tag, void * ret_addr, F f, Args... args ){
	//assumes that ret_addr is a valid address for storing ResultType *
	_ret_addr = ret_addr;
	using ResultType = typename std::result_of< F( Args... ) >::type;
	ResultType _result;
	_fun = std::function<void()>( [ this, f, args... ] ()->void {
		ResultType result = f( args... );
		ResultType * ret_cast = ( ResultType * )this->_ret_addr;
		if( ret_cast )
		    *ret_cast = result;
		return;
	    } );
    }
    template< class F, class... Args >
    inline void set_aux( task_0_impl::RetVoid tag, void * ret_addr, F f, Args... args ){
	_fun = std::function<void()>( [ f, args... ] ()->void {
		f( args... );
		return;
	    } );
    }	
    std::function<void()> _fun;
    void * _ret_addr;
};

class task_0 : public ::e2::interface::i_task< task_0_impl > {};

} }

#endif
