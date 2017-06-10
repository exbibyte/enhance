#ifndef E2_I_TRAIT_GENERAL_HPP
#define E2_I_TRAIT_GENERAL_HPP

namespace e2 { namespace trait {

template< class T >
struct trait_is_renderfrontend {
    constexpr static bool _val = false;
};
template< class T >
struct trait_is_renderbackend {
    constexpr static bool _val = false;
};

//helper to check trait of an instance
template< template<class> class Trait, class Instance >
inline bool instance_has_trait( Instance * inst ){
    return Trait<Instance>::_val;
}

} }

#endif
