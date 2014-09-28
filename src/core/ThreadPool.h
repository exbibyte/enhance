#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <deque>
#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>
#include <iostream>

//unpacking tuple solution from http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

// template <typename ...Args>
// struct save_it_for_later
// {
//   std::tuple<Args...> params;
//   double (*func)(Args...);

//   double delayed_dispatch()
//   {
//     return callFunc(typename gens<sizeof...(Args)>::type());
//   }

//   template<int ...S>
//   double callFunc(seq<S...>)
//   {
//     return func(std::get<S>(params) ...);
//   }
// };

//end of unpacking tuple solution

class FuncWrap {
public:

  class FuncBase {
  public:
    virtual void Call() = 0;
    virtual ~FuncBase(){}
  };
  
  std::unique_ptr< FuncBase > _func;

  template < typename F , typename ... Args >
  class FuncImpl : public FuncBase {
  public:
    F _f;   
    std::tuple<Args... > _params;

    void delayed_dispatch()
    {
      callFunc( typename gens<sizeof...(Args)>::type());
    }

    template< int ...S >
    void callFunc( seq<S...> )
    {
      _f(std::get<S>(_params) ...);
    }

    FuncImpl( F && f, Args && ... args ) : _f( std::move( f ) ), _params( std::make_tuple< Args... > ( std::forward<Args>(args)... ) )
    {
    }
    // void Call() { std::cout<<"number of arguments: "<< std::tuple_size<decltype(_params)>::value << std::endl;}
    void Call() { delayed_dispatch(); }

  };

                template < typename F , typename ... Args >
                FuncWrap( F && f , Args && ... params ) : _func( new FuncImpl< F, Args... >(std::move( f ), std::forward<Args>(params)... ) ){}
                FuncWrap() = default;
                FuncWrap( FuncWrap && other ): _func( std::move( other._func ) ){}
  void          operator() () { _func->Call(); }
  FuncWrap &    operator= ( FuncWrap && other ) {
                  _func = std::move( other._func );
		  return *this;
                }
                FuncWrap( const FuncWrap & ) = delete;
                FuncWrap( FuncWrap & ) = delete;
  FuncWrap &    operator= ( const FuncWrap & ) = delete;
};

class ThreadPool {
public:
  // std::deque < FuncWrap > _queue;

  virtual void TaskAction( FuncWrap & fw ){};

  template < typename FuncType, typename ... Args >
  std::future < typename std::result_of< FuncType( Args... ) >::type > Submit( FuncType f, Args ... params)
  {
    typedef typename std::result_of< FuncType( Args... ) >::type result_type;
    std::packaged_task< result_type( Args... ) > task( std::move( f ) );
    std::future< result_type > res( task.get_future() );
    
    //either this or the commented line below
    FuncWrap fw( std::move(task), std::forward<Args>(params)... );
    
    //call implemented function
    // _queue.push_front( std::move(fw) );
    TaskAction( fw );

    return res;
  }
  virtual bool GetTask( FuncWrap & fw ){ return false; }
};



#endif
