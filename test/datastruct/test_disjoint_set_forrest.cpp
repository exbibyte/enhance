#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "disjoint_set_forrest.hpp"

#include <queue>
#include <map>
#include <functional>
#include <iostream>
using namespace std;

TEST_CASE( "disjoint_set_forrest", "[disjoint_set_forrest]" ) {
      
  bool bRet;

  disjoint_set_forrest::SetNode * n1, * n2, * n3, * n4, * n5 = nullptr;
  bRet = disjoint_set_forrest::MakeSet( n1 );
  bRet &= disjoint_set_forrest::MakeSet( n2 );
  bRet &= disjoint_set_forrest::MakeSet( n3 );
  bRet &= disjoint_set_forrest::MakeSet( n4 );
  bRet &= disjoint_set_forrest::MakeSet( n5 );
  SECTION( "make set result" ) {
      CHECK( bRet );
      CHECK( nullptr != n1 );
      CHECK( nullptr != n2 );
      CHECK( nullptr != n3 );
      CHECK( nullptr != n4 );
      CHECK( nullptr != n5 );
  }  
  SECTION( "union and link, find set, is in same set" ) {
      bRet = disjoint_set_forrest::Union( n1, n2 );
      bRet &= disjoint_set_forrest::Union( n3, n4 );
      CHECK( bRet );

      disjoint_set_forrest::SetNode * n_find;
      n_find = disjoint_set_forrest::FindSet( n1 );
      CHECK( n_find == n1 );
      n_find = disjoint_set_forrest::FindSet( n2 );
      CHECK( n_find == n1 );
      n_find = disjoint_set_forrest::FindSet( n3 );
      CHECK( n_find == n3 );
      n_find = disjoint_set_forrest::FindSet( n4 );
      CHECK( n_find == n3 );
      n_find = disjoint_set_forrest::FindSet( n5 );
      CHECK( n_find == n5 );

      bRet = disjoint_set_forrest::IsSameSet( n1, n2 );
      CHECK( bRet );
      bRet = disjoint_set_forrest::IsSameSet( n1, n4 );
      CHECK( !bRet );
  }  
}
