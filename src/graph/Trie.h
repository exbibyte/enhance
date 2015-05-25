#ifndef TRIE_H
#define TRIE_H

#include <algorithm>
#include <map>
#include <queue>

template< typename KeyType, typename DataType >
class NodeTrie {
    NodeTrie(){
	_is_root = false;
	_is_data_valid = false;
    }
    std::map< KeyType, NodeTrie * > _MapSubNode;
    DataType _data;
    bool _is_root;
    bool _is_data_valid;
};

template< typename KeyType, typename DataType >
class Trie{
public:
    Trie(){
	_proot = &_root;
	_root._is_root = true;
    }
    ~Trie(){
	for( auto i : _root._MapSubNode ){
	    RemoveSubBranch( i );
	}
    }
    bool AddFromRoot( std::queue< KeyType > keys, DataType & data ){
	return Add( _proot, keys, data );
    }
    void RemoveFromRoot( std::queue< KeyType > keys ){
	return Remove( keys );
    }
    void ClearAll( NodeTrie * & node ){
	for( auto i : _proot->_MapSubNode ){	    
	    RemoveSubBranch( i );
	}
	_proot->_MapSubNode.clear();
    }
    bool Add( NodeTrie * node, std::queue< KeyType > keys, DataType & data ){
	if( keys.empty() ){
	    //save data
	    node->_data = data;
	    node->_is_data_valid = true;
	    return true;
	}	
	KeyType current_key = key.front();
	keys.pop();	
	auto found = node->_MapSubNode.find( current_key );
	NodeTrie * subnode;
	if( found == _MapSubNode.end() ){
	    //create new subnode if it does not exist
	    subnode = new NodeTrie;
	    node->_MapSubNode[ current_key ] = subnode;
	}
	else
	{
	    subnode = found->second;
	}
	return Add( subnode, keys, data );
    }
    void Remove( NodeTrie * node, std::queue< KeyType > keys ){
	if( keys.empty() ){
	    //save data
	    node->_is_data_valid = false;
	    return;
	}	
	KeyType current_key = key.front();
	keys.pop();	
	auto found = node->_MapSubNode.find( current_key );
	NodeTrie * subnode;
	if( found == _MapSubNode.end() ){
	    //return if it does not exist
	    return;
	}
	else
	{
	    subnode = found->second;
	}
	return Remove( subnode, keys );
    }
    void RemoveSubBranch( NodeTrie * & node ){	
	for( auto i : node->_MapSubNode ){
	    RemoveChildren( i );
	}
	node->_MapSubNode.clear();
	delete node;
	node = 0;
    }
    bool Get( NodeTrie * node, std::queue< KeyType > keys, DataType & data ){
	if( keys.empty() ){
	    //return data
	    if( node->_is_data_valid ){
		data = node->_data;
		return true;
	    }else
	    {
		return false;
	    }
	}	
	KeyType current_key = key.front();
	keys.pop();	
	auto found = node->_MapSubNode.find( current_key );
	NodeTrie * subnode;
	if( found == _MapSubNode.end() ){
	    //return if it does not exist
	    return false;
	}
	else
	{
	    subnode = found->second;
	}
	return Get( node, keys, data );
    }
private:
    NodeTrie _root;
    NodeTrie * _proot;
}

#endif
