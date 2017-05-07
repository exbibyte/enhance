#ifndef E2_I_GC_HAZARDS_HPP
#define E2_I_GC_HAZARDS_HPP

#include <utility>
#include <functional>
#include <list>
#include <cstring>

namespace e2 { namespace interface {
	
template< class T, class Impl >
class i_gc_hazards : virtual public Impl {
public:
         //container and value traits
	 using type_gc_val =         T;
         using type_gc_impl =        Impl;

               template< class... Args >
               i_gc_hazards( Args && ... args ) : Impl( std::forward< Args >( args )... ) {}
   
               //gc thread specific starts
   inline bool gc_h_dump_global_garbage(){ return Impl::gc_h_dump_global_garbage(); }
   inline bool gc_h_dump_global_hazards(){ return Impl::gc_h_dump_global_hazards(); }
   inline bool gc_h_garbage_clear(){ return Impl::gc_h_garbage_clear(); }
               //gc thread helper functions starts
   inline bool gc_h_hazard_find( T * t ){ return Impl::gc_h_hazard_find( t ); }
               //gc thread helper functions ends    
               //gc thread specific ends

               //normal thread starts
   inline bool gc_h_garbage_clear_force(){ return Impl::gc_h_garbage_clear_force(); }
   inline bool gc_h_garbage_publish(){ return Impl::gc_h_garbage_publish(); }
   inline bool gc_h_hazards_publish(){ return Impl::gc_h_hazards_publish(); }
   inline bool gc_h_hazard_add( T * t ){ return Impl::gc_h_hazard_add( t ); }
   inline bool gc_h_hazard_remove( T * t ){ return Impl::gc_h_hazard_remove( t ); }
               //normal thread ends

               //common starts
   inline bool gc_h_stat_recycled(){ return Impl::gc_h_stat_recycled(); }
               //common ends
};

} }

#endif
