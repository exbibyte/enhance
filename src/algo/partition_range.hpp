#ifndef PARTITION_RANGE_H
#define PARTITION_RANGE_H

#include <list>
#include <utility>

class partition_range {
public:
    struct ele_range{
	int _start;
	int _end;
    };
    class comp{
    public:
	bool operator()( ele_range const & a, ele_range const & b ) const{
	    if( a._end < b._start ){
		return true;
	    }else{
		return false;
	    }
	}
    };
    static void partition( std::list<std::pair<int,int>> & input_ranges, std::list<std::pair<int,int>> & output );
};

#endif
