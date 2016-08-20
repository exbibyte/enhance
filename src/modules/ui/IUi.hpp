#ifndef IUI_H
#define IUI_H

#include <string>
#include <list>
#include <utility>

class IUi {
public:
    using handle_resource = void *;
    struct coordinate {
	handle_resource _handle_resource;
	double _a;
	double _b;
	double _c;
    };
    struct character {
	handle_resource _handle_resource;
	char _character;
    };
    virtual ~IUi(){}
    virtual bool get_coordinates_3( std::list<coordinate> & ) { return false; }
    virtual bool get_characters( std::list<character> & ) { return false; }
    virtual bool init() { return false; }
    virtual bool deinit() { return false; }
    virtual bool register_resource_to_monitor( void * ){ return false; }
};

#endif
