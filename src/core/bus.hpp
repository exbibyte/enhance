#ifndef STREAM_H
#define STREAM_H

class stream {
public:
    enum class eProcessArgType {
	START = 0,
	END,
	EMPTY_STREAM,
	ADD_INSPECTOR,
	REMOVE_INSPECTOR,
	RESET_INSPECTOR,
	GET_STAT,
    };
    class unit {
    public:
	enum {
	    char _str[256];
	    double _num;
	    void * _ptr;
	};
	eProcessArgType _type_request;
    };
    bool init();
    bool deinit();
    template< typename TypePacket >
    unit process( eProcessArgType );
};

#endif
