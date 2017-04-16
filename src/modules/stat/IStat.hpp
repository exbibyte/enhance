#ifndef ISTAT_H
#define ISTAT_H

#include <string>

class IStat {
public:
    enum class e_param {
	register_context,
	deregister_context,
	enable,
	disable,
        query,
        reset,
        context_enter,
	context_exit,
	set_sample_window,
	set_sample_period,
	sample,
    };
    virtual ~IStat(){}
    virtual std::pair< bool, uint64_t > process( e_param, uint64_t in ){ return { false, 0 }; }
};

#endif
