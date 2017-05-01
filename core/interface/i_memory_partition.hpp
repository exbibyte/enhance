#ifndef E2_I_MEMORY_PARTITION_HPP
#define E2_I_MEMORY_PARTITION_HPP


namespace e2 { namespace trait { namespace memory {

enum class e_partition_method {
    none,
    size,
    type,
};

} } }

namespace e2 { namespace interface {

template< class Impl >
class i_memory_partition : public virtual Impl {
public:

               template< class ... Args >
	       i_memory_partition( Args && ... args ) : Impl( std::forward( std::move( args ) )... ) {}

        size_t stat_size_total()                       { return Impl::stat_size_total(); }

        size_t stat_free_size_total()                  { return Impl::stat_free_size_total(); }
        double stat_free_size_mean()                   { return Impl::stat_free_size_mean(); }
        size_t stat_free_count_blocks()                { return Impl::stat_free_count_blocks(); }

        size_t stat_lent_size_total()                  { return Impl::stat_lent_size_total(); }
        double stat_lent_size_mean()                   { return Impl::stat_lent_size_mean(); }
        size_t stat_lent_count_blocks()                { return Impl::stat_lent_count_blocks(); }

        double stat_free_fraction()                    { return Impl::stat_free_fraction(); }

               //reallocation
               template< class ... Args >
          bool internal_resize( Args && ... args )     { return Impl::internal_resize( std::forward< Args >( args )... ); }

               //deallocate to system
               template< class ... Args >
          bool internal_free( Args && ... args )       { return Impl::internal_free( std::forward< Args >( args )... ); }
};

} }

#endif
