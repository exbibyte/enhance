#include "suffix_tree.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <list>
#include <cstdlib>

using namespace std;

bool suffix_tree::edge::has_common_prefix( string const & text, int index ){
    bool has_common = false;
    if( text[index] == text[start] ){
	has_common = true;
    }
    return has_common;
}
pair<int,int> suffix_tree::edge::find_longest_prefix( string const & text, int index ){
    int i = 0;
    while(1){
	if( index >= text.size() || (start + i >= start + len) ){
	    break;
	}
	if( text[index] != text[start+i] ){
	    break;
	}
	++i;
	++index;
    }
    return std::pair<int,int>( index, start + i ); 
}
// string get_edge_text( string const & text ){
//     return text.substr(start,len);
// }
void suffix_tree::node::find_branch( vector<node> & nodes, vector<edge> & edges, string const & text, int index ){
    if( index >= text.size() )
	return;
    bool found = false;
    for( auto & j :_branches ){
	edge & e = edges[j];
	if( e.has_common_prefix( text, index ) ){
	    found = true;
	    // cout << index << ": has common prefix. edge: " << e.get_edge_text( text ) << ", common start: " << e.start << " ";
	    auto longest_pair = e.find_longest_prefix( text, index );
	    int longest_in_edge = longest_pair.second;
	    int longest = longest_pair.first;
	    // cout << "common end: " << longest_in_edge;
	    // cout << ", input string start: " << index << endl;

	    //check if current string input can be further processed down by descendants
	    if( longest_in_edge == e.start + e.len ){
		//don't need to break this edge, continue on to descendants
		node & n_next = nodes[e.next_node];
		// cout << "recurse: longest: " << longest << endl;
		return n_next.find_branch( nodes, edges, text, longest );
	    }else{
		// break this original edge into 2
		int original_end = e.start + e.len;
		int next_node = e.next_node;
		    
		node new_n{};
		    
		nodes.push_back(new_n);
		int index_new_n = nodes.size()-1;
		e.next_node = index_new_n;
		e.len = longest_in_edge - e.start;
		// cout << "split 1: " << text.substr(e.start, e.len) << endl;
		    
		edge new_e;
		new_e.start = longest_in_edge;
		new_e.len = original_end - longest_in_edge;
		new_e.next_node = next_node;
		edges.push_back(new_e);
		nodes.back()._branches.push_back(edges.size()-1);
		// cout << "split 2: " << text.substr(new_e.start, new_e.len) << endl;

		if( longest != text.size() ){
		    //branch another node and edge from the previously created node
		    node new_n2;
		    edge new_e2;
		    new_e2.start = longest;
		    new_e2.len = text.size() - longest;
		    // cout << "branching : " << text.substr(new_e2.start, new_e2.len) << endl;
		    int index_new_n2 = nodes.size();
		    new_e2.next_node = index_new_n2;
		    edges.push_back(new_e2);
		    nodes.back()._branches.push_back(edges.size()-1);
		    nodes.push_back(new_n2);
		}
		return;
	    }
	}
    }
    if( !found ){
	//create new edge and node
	edge e;
	e.start = index;
	e.len = text.size() - index;
	// cout << index << ": create new edge and node: " << text.substr(e.start,e.len) << endl;
	node n;
	nodes.push_back(n);
	e.next_node = nodes.size()-1;
	edges.push_back(e);
	_branches.push_back(edges.size()-1);
    }
}
void suffix_tree::node::travel( vector<node> & nodes, vector<edge> & edges, string const & text, string append, std::function<void(string)> accum ){
    //depth first travel and collect all edges
    for( auto & b : _branches ){
	edge & e = edges[b];
	node & n = nodes[e.next_node];
	string edge_str = text.substr(e.start, e.len);
	string append_temp;
	// string append_temp = append + "-";
	// append_temp += edge_str;
	// char buf[256];
	// sprintf( buf, "%d", e.start );
	// edge_str += ", start:";
	// edge_str += buf;
	accum( edge_str );
	// cout << append_temp << endl;
	n.travel( nodes, edges, text, append_temp, accum );
    }
}

vector<string> suffix_tree::ComputeSuffixTreeEdges( string const & text ) {
    vector<string> result{};
    vector<node> nodes; 
    nodes.reserve(5000*5000);
    vector<edge> edges;
    edges.reserve(5000*5000);
    nodes.push_back({}); // init root
    //add suffixes to tree
    for( int i = 0; i < text.size(); ++i ){
	nodes.front().find_branch( nodes, edges, text, i );
    }
    //collect edges
    auto accum = [&]( string str ){
    	result.push_back(str);
    };
    nodes.front().travel( nodes, edges, text, "", accum );
    return result;
}
