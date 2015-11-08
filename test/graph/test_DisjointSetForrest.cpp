#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "DisjointSetForrest.h"

#include <queue>
#include <map>
#include <functional>
#include <iostream>
using namespace std;

TEST_CASE( "DisjointSetForrest", "[DisjointSetForrest]" ) {
      
  bool bRet;

  DisjointSetForrest::SetNode * n1, * n2, * n3, * n4, * n5 = nullptr;
  bRet = DisjointSetForrest::MakeSet( n1 );
  bRet &= DisjointSetForrest::MakeSet( n2 );
  bRet &= DisjointSetForrest::MakeSet( n3 );
  bRet &= DisjointSetForrest::MakeSet( n4 );
  bRet &= DisjointSetForrest::MakeSet( n5 );
  SECTION( "make set result" ) {
      CHECK( bRet );
      CHECK( nullptr != n1 );
      CHECK( nullptr != n2 );
      CHECK( nullptr != n3 );
      CHECK( nullptr != n4 );
      CHECK( nullptr != n5 );
  }  
  SECTION( "union and link, find set" ) {
      bRet = DisjointSetForrest::Union( n1, n2 );
      bRet &= DisjointSetForrest::Union( n3, n4 );
      CHECK( bRet );

      DisjointSetForrest::SetNode * n_find;
      n_find = DisjointSetForrest::FindSet( n1 );
      CHECK( n_find == n1 );
      n_find = DisjointSetForrest::FindSet( n2 );
      CHECK( n_find == n1 );
      n_find = DisjointSetForrest::FindSet( n3 );
      CHECK( n_find == n3 );
      n_find = DisjointSetForrest::FindSet( n4 );
      CHECK( n_find == n3 );
      n_find = DisjointSetForrest::FindSet( n5 );
      CHECK( n_find == n5 );

  }  
}
