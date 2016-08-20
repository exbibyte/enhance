#ifndef UI0_H
#define UI0_H

#include "IUi.hpp"

#include <GLFW/glfw3.h>

#include <set>
#include <list>
#include <unordered_map>

class UiMouse0 : public IUi {
public:
    char const * get_id(){ return "uimouse0"; }
    bool get_coordinates_3( std::list<coordinate> & );
    bool get_characters( std::list<character> & );
    bool init();
    bool deinit();
    bool register_resource_to_monitor( handle_resource ); //resource handle is assumed to be GLFwindow *
private:
    static void process_mouse_move( GLFWwindow * window, double xpos, double ypos );
    std::set< handle_resource > _resources_to_monitor;
    std::list<coordinate> _coords;
    std::list<character> _chars;
    static std::unordered_multimap<handle_resource, UiMouse0*> _map_resource_to_instance;
};

#endif
