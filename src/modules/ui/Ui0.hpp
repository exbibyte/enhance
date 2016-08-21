#ifndef UI0_H
#define UI0_H

#include "IUi.hpp"

#include "GLIncludes.hpp"

#include <set>
#include <list>
#include <unordered_map>

class Ui0 : public IUi {
public:
    char const * get_id(){ return "ui0"; }
    bool get_characters( std::list<character> & );
    bool init();
    bool deinit();
    bool register_resource_to_monitor( handle_resource ); //resource handle is assumed to be GLFwindow *
private:
    static void process_mouse_move( GLFWwindow * window, double xpos, double ypos );
    static void process_mouse_button( GLFWwindow * window, int button, int action, int mods );
    static void process_key_input( GLFWwindow * window, int key, int scancode, int action, int mods );
    std::set< handle_resource > _resources_to_monitor;
    std::list<character> _chars;
    static std::unordered_multimap<handle_resource, Ui0*> _map_resource_to_instance;
};

#endif
