#ifndef E2_I_KERNEL_HPP
#define E2_I_KERNEL_HPP

namespace e2 { namespace interface {

template< class Impl >
class i_kernel : public Impl {
public:
                i_kernel & operator =( i_kernel const & ) = delete;
               inline bool kernel_init() { return Impl::kernel_init(); }
               inline bool kernel_run(){ return Impl::kernel_run(); }
               inline bool kernel_deinit(){ return Impl::kernel_deinit(); }
};

} }

#endif
