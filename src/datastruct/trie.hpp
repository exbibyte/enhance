#ifndef TRIE_H
#define TRIE_H

#include <algorithm>
#include <map>
#include <queue>
#include <set>

template< typename KeyType, typename DataType >
class node_trie {
public:
    node_trie(){
	_is_root = false;
	_is_data_valid = false;
    }
    std::map< KeyType, node_trie<KeyType, DataType> * > _MapSubNode;
    DataType _data;
    bool _is_root;
    bool _is_data_valid;
};

template< typename KeyType, typename DataType >
class trie{
public:
    trie(){
	_proot = &_root;
	_root._is_root = true;
    }
    ~trie(){
	for( auto i : _root._MapSubNode ){
	    RemoveSubBranch( i.second );
	}
    }
    bool AddFromRoot( std::queue< KeyType > keys, DataType data ){
	return Add( _proot, keys, data );
    }
    void RemoveFromRoot( std::queue< KeyType > keys ){
	return Remove( _proot, keys );
    }
    void ClearAll(){
	for( auto i : _proot->_MapSubNode ){	    
	    RemoveSubBranch( i.second );
	}
	_proot->_MapSubNode.clear();
    }
    bool GetFromRoot( std::queue< KeyType > keys, DataType & data ){
        return Get( _proot, keys, data );
    }
    bool Add( node_trie<KeyType, DataType> * node, std::queue< KeyType > keys, DataType data ){
	if( keys.empty() ){
	    //save data
	    node->_data = data;
	    node->_is_data_valid = true;
	    return true;
	}	
	KeyType current_key = keys.front();
	keys.pop();	
	auto found = node->_MapSubNode.find( current_key );
	node_trie<KeyType, DataType> * subnode;
	if( found == node->_MapSubNode.end() ){
	    //create new subnode if it does not exist
	    subnode = new node_trie< KeyType, DataType >;
	    node->_MapSubNode[ current_key ] = subnode;
	}
	else
	{
	    subnode = found->second;
	}
	return Add( subnode, keys, data );
    }
    void Remove( node_trie<KeyType, DataType> * node, std::queue< KeyType > keys ){
	if( keys.empty() ){
	    //save data
	    node->_is_data_valid = false;
	    return;
	}	
	KeyType current_key = keys.front();
	keys.pop();	
	auto found = node->_MapSubNode.find( current_key );
	node_trie<KeyType, DataType> * subnode;
	if( found == node->_MapSubNode.end() ){
	    //return if it does not exist
	    return;
	}
	else
	{
	    subnode = found->second;
	}
	return Remove( subnode, keys );
    }
    void RemoveSubBranch( node_trie<KeyType, DataType> * node ){	
	for( auto i : node->_MapSubNode ){
	    RemoveSubBranch( i.second );
	}
	node->_MapSubNode.clear();
	delete node;
        node = 0;
    }    
    bool Get( node_trie<KeyType, DataType> * node, std::queue< KeyType > keys, DataType & data ){
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
	KeyType current_key = keys.front();
	keys.pop();
	auto found = node->_MapSubNode.find( current_key );
	node_trie<KeyType, DataType> * subnode;
	if( found == node->_MapSubNode.end() ){	    
            if( !node->_is_data_valid ){
                //return if it does not exist and current node doesn't contain valid data
                return false;
	    }else{
                //return current node data if it is valid
		data = node->_data;
                return true;
            }		
	}
	else
	{
	    subnode = found->second;
	}
	return Get( subnode, keys, data );
    }
    void GetPartialFromRoot( std::set< KeyType > keys, std::vector< DataType > & data ){
	data.clear();
	GetPartial( _proot, keys, data );
    }
    void GetPartial( node_trie<KeyType, DataType> * node, std::set< KeyType > keys, std::vector< DataType > & data ){
	if( node->_is_data_valid ){
	    //save data if node stores valid data
	    data.push_back( node->_data );
	}
	if( keys.empty() ){	    
	    return;
	}
	//find if key(s) to next sub-branch(es) exist
	for( auto i : node->_MapSubNode ){
	    KeyType search_key = i.first;
	    std::set< KeyType > keys_copy = keys;
	    auto j = keys_copy.find( search_key );
	    if( j != keys_copy.end() ){
		//found, remove the found key in set
		KeyType found_key = *j;
		keys_copy.erase(j);
		//search in sub-branch
		node_trie<KeyType, DataType> * subnode = i.second;
		GetPartial( subnode, keys_copy, data );
	    }
	}
    }
private:
    node_trie<KeyType, DataType> _root;
    node_trie<KeyType, DataType> * _proot;
};

#endif
