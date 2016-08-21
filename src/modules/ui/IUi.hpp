#ifndef IUI_H
#define IUI_H

#include <string>
#include <list>
#include <utility>

class IUi {
public:
    using key_character = char;
    enum class input_type {
	KEY,
	MOUSE,
	MOUSE_COORD,
	OTHER,
    };
    enum class mouse_character {
	LEFT,
	RIGHT,
	MID,
	OTHER,
    };
    enum class state {
	DOWN,
	UP,
	REPEAT, //applicable to key input
	OTHER,
    };
    using handle_resource = void *;
    struct coordinate {
	double _a;
	double _b;
	double _c;
    };
    struct character {
	handle_resource _handle_resource;
	input_type _input_type;
        union {
	    key_character _key_character;
	    mouse_character _mouse_character;
	    coordinate _coordinate;
	};
	state _state;
    };
    virtual ~IUi(){}
    virtual bool get_characters( std::list<character> & ) { return false; }
    virtual bool init() { return false; }
    virtual bool deinit() { return false; }
    virtual bool register_resource_to_monitor( handle_resource ){ return false; }
};

#endif
