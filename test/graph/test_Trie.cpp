#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Trie.h"

#include <queue>
#include <map>
#include <functional>
#include <iostream>
using namespace std;

bool test_callback1(){
  cout <<"test callback 1" << endl;
  return true;
}
bool test_callback2(){
  cout <<"test callback 2" << endl;
  return true;
}
bool test_callback3(){
  cout <<"test callback 3" << endl;
  return false;
}

TEST_CASE( "Trie", "[Trie]" ) {
      
  bool bRet;
  Trie<int, std::function<bool(void)> > _trie;

  SECTION( "insert and retrieve items" ) {
    queue< int > keys1;
    keys1.push(3);
    keys1.push(1);
    
    queue< int > keys2;
    keys2.push(3);
    keys2.push(5);
    keys2.push(0);
    
    queue< int > keys3;
    keys3.push(7);
    
    bRet = _trie.AddFromRoot( keys1, &test_callback1 );
    CHECK( bRet );
    bRet = _trie.AddFromRoot( keys2, &test_callback2 );
    CHECK( bRet );
    bRet = _trie.AddFromRoot( keys3, &test_callback3 );
    CHECK( bRet );
    
    std::function<bool(void)> _f;

    bRet = _trie.GetFromRoot( keys1, _f );
    CHECK( bRet );
    bRet = _f();
    CHECK( bRet );

    bRet = _trie.GetFromRoot( keys3, _f );
    CHECK( bRet );
    bRet = _f();
    CHECK( bRet );

    bRet = _trie.GetFromRoot( keys2, _f );
    CHECK( bRet );
    bRet = _f();
    CHECK( !bRet );
  }  

  SECTION( "remove items" ) {
    queue< int > keys1;
    keys1.push(3);
    keys1.push(1);
    
    queue< int > keys2;
    keys2.push(3);
    keys2.push(5);
    keys2.push(0);
        
    bRet = _trie.AddFromRoot( keys1, &test_callback1 );
    CHECK( bRet );
    bRet = _trie.AddFromRoot( keys2, &test_callback2 );
    CHECK( bRet );
    
    _trie.ClearAll();

    bRet = _trie.GetFromRoot( keys1, _f );
    CHECK( !bRet );
    bRet = _trie.GetFromRoot( keys2, _f );
    CHECK( !bRet );
  }  
}
