#include "partition_range.hpp"

#include <set>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void partition_range::partition( list<pair<int,int>> & input_ranges, list<pair<int,int>> & output ){
    output.clear();
    std::set< partition_range::ele_range, partition_range::comp> grid {};
    for( auto & i : input_ranges ){
        ele_range c;
        c._start = i.first;
        c._end = i.second;
        //find overlapping range due to existing ranges
        auto it_overlap_start = grid.lower_bound(c);
        auto it_overlap_end = grid.upper_bound(c);
        if( it_overlap_start != grid.end() ){
            auto it_erase_start = it_overlap_start;
            auto it_erase_end = it_overlap_end;
            list<ele_range> overlapped {}; //to be deleted
            list<ele_range> new_ranges{}; //to be inserted
            while( it_overlap_start != it_overlap_end ){
                overlapped.push_back(*it_overlap_start);
                ++it_overlap_start;
            }
            overlapped.push_back(c);

            vector<int> overlapped_index{}; //intermediate data
            overlapped_index.reserve(2*overlapped.size());
            for( auto & h : overlapped ){
                overlapped_index.push_back(h._start);
                overlapped_index.push_back(h._end);
            }
            sort(overlapped_index.begin(), overlapped_index.end());
            auto it_new_end = std::unique(overlapped_index.begin(), overlapped_index.end() );
            //resize
            overlapped_index.resize(std::distance(overlapped_index.begin(), it_new_end));

	    auto it_h_start = overlapped_index.begin(), it_h_end = overlapped_index.end();
            if(overlapped_index.size() == 1){ //only 1 element
                ele_range n1;
                n1._start = *it_h_start;
                n1._end = *it_h_start;
                new_ranges.push_back( n1 );
                // cout << "new range: " << n1._start << ", " << n1._end << endl;
            }else{ // >= 2 elements
                while( it_h_start != it_h_end ){
                    //create new ranges from sorted indices
                    auto it_next = std::next(it_h_start);
		    auto it_next_next = std::next(it_next);
                    if( it_next_next != it_h_end ){
                        ele_range n2;
                        n2._start = *it_h_start;
                        n2._end = (*it_next - 1);
                        new_ranges.push_back( n2 );
                        // cout << "new range: " << n2._start << ", " << n2._end << endl;
                    }else{ //process the last range
                        ele_range n3;
                        n3._start = *it_h_start;
                        n3._end = *it_next;
                        new_ranges.push_back( n3 );
                        // cout << "new range: " << n3._start << ", " << n3._end << endl;
			break;
                    }
                    ++it_h_start;
                }
            }
            //remove old overlapped ranges
            grid.erase( it_erase_start, it_erase_end );
            //insert new non-overlapped ranges;
            for( auto & n : new_ranges ){
		grid.insert( it_overlap_end, n );
            }
        }else{
            //new range is non-overlapping, so just insert it
            grid.insert( c );
        }
    }
    for( auto & j : grid ){
        output.push_back( std::pair<int,int>( j._start, j._end ) );
    }
}
