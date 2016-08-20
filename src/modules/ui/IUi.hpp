#ifndef IUI_H
#define IUI_H

#include <string>
#include <list>
#include <utility>

class IUi {
public:
    using key_character = char;
    enum class character_type {
	KEY,
	MOUSE,
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
	handle_resource _handle_resource;
	double _a;
	double _b;
	double _c;
    };
    struct character {
	handle_resource _handle_resource;
	character_type _character_type;
        union {
	    key_character _key_character;
	    mouse_character _mouse_character;
	};
	state _state;
    };
    virtual ~IUi(){}
    virtual bool get_coordinates_3( std::list<coordinate> & ) { return false; }
    virtual bool get_characters( std::list<character> & ) { return false; }
    virtual bool init() { return false; }
    virtual bool deinit() { return false; }
    virtual bool register_resource_to_monitor( void * ){ return false; }
};

#endif
