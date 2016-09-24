#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <iostream>
#include <queue>

class huffman{
public:
    struct node {
        int c;
        int count;
        node * left;
        node * right;
    };
    class comp{
    public:
        bool operator()( node * const & a, node * const & b ){
            return a->count > b->count;
        }
    };
    huffman(){
        reset();
    }
    node * generate_tree( std::string input ){
        reset();
        for( size_t i = 0; i < input.size(); ++i ){
            ++letters[tolower(input[i])-'a'].count;
        }
        std::priority_queue<node*, std::vector<node*>, huffman::comp > q;
        for( size_t i = 0; i < 26; ++i ){
            if( letters[i].count > 0 )
                q.push( &letters[i] );
        }
        while( q.size() > 1 ){
            node * n1 = q.top();
            q.pop();
            node * n2 = q.top();
            q.pop();
            internals[index_internals].count = n1->count + n2->count;
            internals[index_internals].left = n1;
            internals[index_internals].right = n2;
            q.push( &internals[index_internals] );
            index_internals++;
        }
        node * root = q.top();
        return root;
    }
    void visit_nodes( node * n, std::vector<int> path ){
        if( !n ) return;
        else if( !n->left && !n->right ){
            std::cout << (char)(n->c + 'a') << ": ";
            for( size_t i = 0; i < path.size(); ++i ){
                std::cout << path[i];
            }
            std::cout << std::endl;
        }else{
            path.push_back(0);
            visit_nodes( n->left, path );
            path.pop_back();
            path.push_back(1);
            visit_nodes( n->right, path );
        }
    }
    void reset(){
        memset( letters, 0, sizeof(letters) );
        memset( letters, 0, sizeof(internals) );
        index_internals = 0;
        for( size_t i = 0; i < 26; ++i ){
            letters[i].c = i;
        }
    }
private:
    int index_internals;
    node letters[26]; //converts input to lower case and stores frequency
    node internals[25]; //upper bounded by 26-1 = 25 in the pathological case (character freqeuncy resembles a fibonacci sequence)
};

#endif
