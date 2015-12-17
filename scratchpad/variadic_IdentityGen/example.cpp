#include <iostream>

#include <type_traits>

using namespace std;

namespace helper
{
    template <int... Is>
    struct index {};

    template <int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

    template <int... Is>
    struct gen_seq<0, Is...> : index<Is...> {};
}

template <typename T, typename... Ts> struct get_index;

template <typename T, typename... Ts>
struct get_index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

template <typename T, typename Tail, typename... Ts>
struct get_index<T, Tail, Ts...> : std::integral_constant<std::size_t, 1 + get_index<T, Ts...>::value> {};


template< typename T >
class Identity{};

class A {
public:
    void TestAction(){
	cout << "action" << endl;
    }
};

class B {
public:
};


class C {
public:
};

class E {
public:
};

template< typename Host >
class IdentityGen : public Host {
public:
    typedef Host HostType;
    static Identity<Host> _identity;
    int _n;
    int GetN( Identity<Host> ){ return _n; }
    IdentityGen( int n = 0 ) : _n( n ) {}
};

template< typename... Injections >
class D{
};

template< typename First, typename... Injections >
class D< First, Injections... > : public First, D< Injections... > {
public:
    using First::GetN;
    using D< Injections... >::GetN;
    
    D( First first, Injections... rest ) : First( first ), D< Injections... >( rest... ) {}
};

template< typename Injection >
class D< Injection > : public Injection {
public:
    D( Injection inj ) : Injection( inj ) {}
    
};

struct Pass {
    template<typename ...T> Pass(T...) {}
};

template < int... Is, typename... Args >
auto GetSequenceHelper( helper::index<Is...>, Args&... args )
{
//    Pass{ ( cout << Is << endl, 1 )... };    
    Pass{ ( [&]{
		args._n = Is;
		//cout << args._n;
	        }(),
	        1
	   )...
	};
    return true;
}

template < template < typename... Args > class Cons, typename... Args >
auto GenSequence( Args... args )
{
    auto sequence = helper::gen_seq<sizeof...(Args)>{};
    GetSequenceHelper( sequence, args... );
    // Cons<Args...> _z( args... );
    Cons<Args...> _z( args... );
    return _z;
}

int main()
{
    // static_assert(0 == get_index<char, char, int, void>::value, "");
    // static_assert(1 == get_index<int, char, int, void>::value, "");
    // static_assert(2 == get_index<void, char, int, void>::value, "");
    auto _z = GenSequence< D >( IdentityGen<A>(), IdentityGen<B>(), IdentityGen<C>(), IdentityGen<E>() );
    _z.TestAction();
    cout << _z.GetN(IdentityGen<A>::_identity) << _z.GetN(IdentityGen<B>::_identity) << _z.GetN(IdentityGen<C>::_identity) << _z.GetN(IdentityGen<E>::_identity) << endl;
    return 0;
}
