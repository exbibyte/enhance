#ifndef I_CONCURRENCY_HPP
#define I_CONCURRENCY_HPP

enum class e_bound_size {
    bounded,
    unbounded,
};

enum class e_method {
    none,
    total,
    partial,
    synchronized,
};

enum class e_granularity {
    none,
    global,
    local,
    lockfree,
    waitfree,
};

struct i_concurrency_traits {
    e_bound_size _bound_size;
    e_method _method;
    e_granularity _granularity;
};

#endif
