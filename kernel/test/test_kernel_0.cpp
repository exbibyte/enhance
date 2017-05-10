#include <cassert>

#include "kernel_0.hpp"

static ::e2::kernel::kernel_0  _kernel;

int main() {

    bool ret;
    
    ret = _kernel.kernel_init();
    assert( ret );
    ret = _kernel.kernel_run();
    assert( ret );
    ret = _kernel.kernel_deinit();
    assert( ret );

    return 0;
}
