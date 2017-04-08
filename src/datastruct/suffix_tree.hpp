#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <list>
#include <cstdlib>

using namespace std;

class suffix_tree {
public:
    class edge {
    public:
	int start;
	int len;
	int next_node;
	bool has_common_prefix( string const & text, int index );
	pair<int,int> find_longest_prefix( string const & text, int index );
    };
    class node {
    public:
	void find_branch( vector<node> & nodes, vector<edge> & edges, string const & text, int index );
	void travel( vector<node> & nodes, vector<edge> & edges, string const & text, string append, std::function<void(string)> accum );
	vector<int> _branches;
    };
    static vector<string> ComputeSuffixTreeEdges( string const & text );
};

#endif
