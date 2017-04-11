#ifndef ICAM_HPP
#define ICAM_HPP

class ICam {
public:
    enum class e_pparam {
	register_ui,
	register_orientationmanip,
	register_filter,
	deregister_ui,
	deregister_orientationmanip,
	deregister_filter,
	disable,
	enable,
	operation,
    };
    enum class operation_mode { //todo: delegate to filter instead
        avg_all_discard_all_after,
        earliest_discard_all_after,
        latest_discard_all_after,
        avg_all_discard_half_after,
        earliest_discard_half_after,
        latest_discard_half_after,	
    };
    virtual ~ICam(){}
    virtual bool init( e_pparam, void * ) { return false; }
    virtual bool deinit( e_pparam, void * ) { return false; }
    virtual bool process( e_pparam, void * ) { return false; }
};

#endif
