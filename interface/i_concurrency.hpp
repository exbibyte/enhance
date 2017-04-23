#ifndef I_CONCURRENCY_HPP
#define I_CONCURRENCY_HPP

namespace e2 { namespace trait { namespace concurrency {

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
    disjoint_access,
    lockfree,
    waitfree,
};

enum class e_fairness {
    not_applicable,
    none,
    fifo,
    lifo,
};

struct trait_concurrency {
    e_bound_size _bound_size;
    e_method _method;
    e_granularity _granularity;
    e_fairness _fairness;
};

} } }

#endif
