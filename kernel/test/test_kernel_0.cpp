#include <cassert>
#include <thread>
#include <chrono>

#include "kernel_0.hpp"

static ::e2::kernel::kernel_0  _kernel;

int main() {

    bool ret;
    
    ret = _kernel.kernel_init();
    assert( ret );
    ret = _kernel.kernel_run();
    std::this_thread::sleep_for( std::chrono::milliseconds(5000) );
    assert( ret );
    ret = _kernel.kernel_deinit();
    assert( ret );

    return 0;
}
