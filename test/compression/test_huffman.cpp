#include <string>
#include <iostream>

#include "huffman.hpp"

using namespace std;

int main(int argc, char ** argv )
{
    string input;
    cin >> input;

    huffman h;
    huffman::node * root = h.generate_tree( input );
    h.visit_nodes( root, vector<int>() );

    return 0;
}

