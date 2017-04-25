#ifndef COMBINATRONICS_HPP
#define COMBINATRONICS_HPP

class combinatronics {
public:
    template< typename T>
    static bool permute_next( T * start, T * end );
};

template< typename T>
bool combinatronics::permute_next( T * start, T * end ){
    T * current = end - 1;
    T swap_item;
    T * rightmost;
    while( (current-1) != start ){ //search backwards for ascending sequence, stop at first occurence where it is not ascending
	if( *(current-1) < *current ){
	    //find rightmost value that comes right after *(current-1)
	    rightmost = current;
	    while( rightmost != end ){
		if( *(current-1) >= *rightmost ){
		    rightmost++;
		}
		else{
		    break;
		}
	    }
	    //swap rightmost with (current-1)
	    swap_item = *(current-1);
	    *(current-1) = *rightmost;
	    *rightmost = swap_item;
	    return true;
	}
	--current;
    }
    return false;
}

#endif
