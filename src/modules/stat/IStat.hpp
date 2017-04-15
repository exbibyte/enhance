#ifndef ISTAT_H
#define ISTAT_H

#include <string>

class IStat {
public:
    enum class e_param {
	register_func,
	deregister_func,
	enable,
	disable,
        query,
        reset,
	operation,
	set_count_window,
    };
    virtual ~IStat(){}
    virtual bool process( e_param, void * in, double & ret ){ return false; }
};

#endif
