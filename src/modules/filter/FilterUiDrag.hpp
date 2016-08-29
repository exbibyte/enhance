#ifndef FILTER_UI_DRAG_H
#define FILTER_UI_DRAG_H

#include "IFilter.hpp"
#include "IUi.hpp"

#include "GLIncludes.hpp"

#include <set>
#include <list>
#include <unordered_map>

struct drag_coordinate {
    IUi::mouse_character _mouse_character;
    IUi::coordinate _coordinate_start;
    IUi::coordinate _coordinate_delta;
};

class FilterUiDrag : public IFilter<drag_coordinate,IUi::character> {
public:
    struct coordinate_cache {
	IUi::coordinate _coordinate;
	bool _active;
    };
    char const * get_id(){ return "filteruidrag"; }
    bool init() override;
    bool deinit() override;
    bool process( std::list<drag_coordinate> & out, std::list<IUi::character> & in ) override;
private:
    std::unordered_map<IUi::handle_resource, std::unordered_map<int,coordinate_cache> > _map_resource_to_charcache;
};

#endif
