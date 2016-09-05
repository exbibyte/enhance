#include "bwt.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

string bwt::transform( string const & text) {
    vector<string> m ( text.size() );
    m[0] = text;
    for( int i = 1; i < text.size(); ++i ){
        m[i] = m[i-1].substr(1, string::npos) + m[i-1].substr(0,1);
    }
    sort( m.begin(), m.end() );

    string result {};
    result.resize(text.size());
    for( int i = 0; i < text.size(); ++i ){
        result[i] = m[i].back();
    }
    return result;
}

string bwt::inverse( string const & input ){
    string text = "";
    //first last property method:
    unordered_map< char, unordered_map< int, int > > map_bwt_index_to_char_order{};
    // map_bwt_index_to_char_order.reserve(5);
    // map_bwt_index_to_char_order['A'] = {};
    // map_bwt_index_to_char_order['C'] = {};
    // map_bwt_index_to_char_order['G'] = {};
    // map_bwt_index_to_char_order['T'] = {};
    // map_bwt_index_to_char_order['$'] = {};
    // map_bwt_index_to_char_order['A'].reserve(1000000);
    // map_bwt_index_to_char_order['C'].reserve(1000000);
    // map_bwt_index_to_char_order['G'].reserve(1000000);
    // map_bwt_index_to_char_order['T'].reserve(1000000);
    // map_bwt_index_to_char_order['$'].reserve(1);
  
    unordered_map< char, unordered_map< int, int > > map_first_column_char_order_to_index{};
    // map_first_column_char_order_to_index.reserve(5);
    // map_first_column_char_order_to_index['A'] = {};
    // map_first_column_char_order_to_index['C'] = {};
    // map_first_column_char_order_to_index['G'] = {};
    // map_first_column_char_order_to_index['T'] = {};
    // map_first_column_char_order_to_index['$'] = {};
    // map_first_column_char_order_to_index['A'].reserve(1000000);
    // map_first_column_char_order_to_index['C'].reserve(1000000);
    // map_first_column_char_order_to_index['G'].reserve(1000000);
    // map_first_column_char_order_to_index['T'].reserve(1000000);
    // map_first_column_char_order_to_index['$'].reserve(1);
    unordered_map<char,int> bwt_char_order {};
    // bwt_char_order.reserve(5);
    unordered_map<char,int> first_column_char_order {};
    // first_column_char_order.reserve(5);
  
    string first_col = input;
    sort( first_col.begin(), first_col.end() );

    //precompute order
    for( int i = 0; i < input.size(); ++i ){
	int count;
	auto it = bwt_char_order.find(input[i]);
	if( it == bwt_char_order.end() ){
	    count = 0;
	}else{
	    count = it->second;
	}
	++count;
	if( map_bwt_index_to_char_order.find( input[i] ) == map_bwt_index_to_char_order.end() ){
	    map_bwt_index_to_char_order[input[i]] = {};
	}
	map_bwt_index_to_char_order[input[i]][i] = count; //<char,row index> -> order
	bwt_char_order[ input[i] ] = count; //update count

	int count_first_col;
	auto it_first_col = first_column_char_order.find( first_col[i] );
	if( it_first_col == first_column_char_order.end() ){
	    count_first_col = 0;
	}else{
	    count_first_col = it_first_col->second;
	}
	++count_first_col;
	if( map_first_column_char_order_to_index.find( first_col[i] ) == map_first_column_char_order_to_index.end() ){
	    map_first_column_char_order_to_index[first_col[i]] = {};
	}
	map_first_column_char_order_to_index[first_col[i]][count_first_col] = i; //<char,order> -> row index
	first_column_char_order[ first_col[i] ] = count_first_col; //update count
    } 

    list<char> ans{};
    int index = 0;
  
    for( int i = 0; i < input.size(); ++i ){
	char first_col_char = first_col[index];
	ans.push_front( first_col_char );
	char last_col_char = input[index];
	//get the order of the character in the bwt column
	int char_order = map_bwt_index_to_char_order[last_col_char][index];      
	//map to first column
	int first_col_index = map_first_column_char_order_to_index[last_col_char][char_order];
	index = first_col_index;
    }
    return string( ans.begin(), ans.end() );
}
