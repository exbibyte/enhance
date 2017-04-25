#ifndef I_LIST_HPP
#define I_LIST_HPP

#include <utility>

namespace e2 {
    namespace interface {
	
	template< class ValType, template<class> class ContainerType >
	class i_list : public virtual ContainerType< ValType > {
	public:
	    using container_type =     ContainerType< ValType >;
	    using value_type =         ValType;
	    using value_ptr =          ValType *;
	    using const_value_ptr =    ValType const *;
	    
	      bool clear(){ return ContainerType< ValType >::clear(); }
	    size_t size(){ return ContainerType< ValType >::size(); }
	    size_t update_size(){ return ContainerType< ValType >::update_size(); }
	      bool push_back( ValType const * item ){ return ContainerType< ValType >::push_back( item ); }
	      bool push_front( ValType const * item ){ return ContainerType< ValType >::push_front( item ); }
	      bool pop_back( ValType * item ){ return ContainerType< ValType >::pop_back( item ); }
	      bool pop_front( ValType * item ){ return ContainerType< ValType >::pop_front( item ); }
	      bool front( ValType * item ){ return ContainerType< ValType >::front( item ); }
	      bool back( ValType * item ){ return ContainerType< ValType >::back( item ); }
	};

	template< class ValType, template<class> class ContainerType, class IterableType >
	class i_spliceable : public virtual ContainerType< ValType > {
	public:
	    using iterator_type = IterableType;
	    
	    bool splice_entire( IterableType * insert_at, ContainerType< ValType > * from_list ){ return ContainerType< ValType >::splice_entire( insert_at, from_list ); }
	    bool splice_single( IterableType * insert_at, ContainerType< ValType > * from_list, IterableType * insert_from ){ return ContainerType< ValType >::splice_single( insert_at, from_list, insert_from ); }
	    bool splice_range( IterableType * insert_at, ContainerType< ValType > * from_list, IterableType * insert_from_iter_begin, IterableType * insert_from_iter_end ){ return ContainerType< ValType >::splice_range( insert_at, from_list, insert_from_iter_begin, insert_from_iter_end ); }
	};

    }
}
#endif
