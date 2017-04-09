#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "heap_fib.hpp"
#include <cassert>
#include <vector>
#include <utility>
#include <list>
#include <random>

using namespace std;

TEST_CASE( "heap_fib basic", "[heap_fib]" ) {

  heap_fib<int> h0;

  //insert 4 items
  void * n0 = h0.insert( 5, 0 );
  void * n1 = h0.insert( 3, 1 );
  void * n2 = h0.insert( -1, 2 );
  void * n3 = h0.insert( 10, 3 );
  
  CHECK( h0.get_num_nodes() == 4 );
  CHECK( h0.get_num_root_trees() == 4 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 4 );

  int key, item;
  bool ret;
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( -1 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 2 == item );

  //extract min
  key = 0;
  item = 0;
  void * invalidated;
  ret = h0.extract_min( key, item, invalidated );
  CHECK( ret );
  CHECK( -1 == key );
  CHECK( 2 == item );
  CHECK( invalidated == n2 );
  CHECK( h0.get_num_nodes() == 3 );
  CHECK( h0.get_num_root_trees() == 2 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 2 );

  //decrease key
  h0.decrease_key( n3, -2 );
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( -2 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 3 == item );

  CHECK( h0.get_num_nodes() == 3 );
  CHECK( (h0.get_num_root_trees() == 2 || h0.get_num_root_trees() == 3) );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( (h0.get_potential() == 2 || h0.get_potential() == 3) );

  h0.remove( n3 );
  CHECK( h0.get_num_nodes() == 2 );
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( 3 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 1 == item );

  h0.remove( n1 );
  CHECK( h0.get_num_nodes() == 1 );
  CHECK( h0.get_num_root_trees() == 1 );
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( 5 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 0 == item );
  

  key = 0, item = 0;
  ret = h0.extract_min( key, item, invalidated );
  CHECK( ret );
  CHECK( h0.get_num_nodes() == 0 );
  CHECK( h0.get_num_root_trees() == 0 );
  CHECK( 5 == key );
  CHECK( 0 == item );
}

TEST_CASE( "heap_fib rand insert removal", "[heap_fib]" ) {
  heap_fib<int> h0;
  default_random_engine gen;
  uniform_int_distribution<int> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max() );
  vector<int> keys{};
  for( int i = 0; i < 1000; ++i ){
    int n = distr(gen);
    keys.push_back(n);
    h0.insert( n, i );
  }
  CHECK( h0.get_num_nodes() == 1000 );
  std::sort( keys.begin(), keys.end() );
  for( auto i : keys ){
    int key, item;
    void * invalidated;
    bool ret = h0.extract_min( key, item, invalidated );
    CHECK( ret );
    CHECK( i == key );
  }
  CHECK( h0.get_num_nodes() == 0 );
  CHECK( h0.get_num_root_trees() == 0 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 0 );
}

TEST_CASE( "heap_fib union", "[heap_fib]" ) {
  heap_fib<int> h0, h1;
  default_random_engine gen;
  uniform_int_distribution<int> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max() );
  vector<int> keys{};
  for( int i = 0; i < 50; ++i ){
    int n = distr(gen);
    keys.push_back(n);
    h0.insert( n, i );
    n = distr(gen);
    keys.push_back(n);
    h1.insert( n, i );
  }
  CHECK( h0.get_num_nodes() == 50 );
  CHECK( h1.get_num_nodes() == 50 );
  std::sort( keys.begin(), keys.end() );
  heap_fib<int> h2 = heap_fib<int>::heap_union( h0, h1 );
  CHECK( h2.get_num_nodes() == 100 );
  CHECK( h0.get_num_nodes() == 0 );
  CHECK( h1.get_num_nodes() == 0 );
  for( auto i : keys ){
    int key, item;
    void * invalidated;
    bool ret = h2.extract_min( key, item, invalidated );
    CHECK( ret );
    CHECK( i == key );
  }
  CHECK( h2.get_num_nodes() == 0 );
  CHECK( h2.get_num_root_trees() == 0 );
  CHECK( h2.get_num_marked_nodes() == 0 );
  CHECK( h2.get_potential() == 0 );
}

TEST_CASE( "heap_fib potential", "[heap_fib]" ) {

  heap_fib<int> h0;

  //insert 4 items
  void * n0 = h0.insert( 5, 0 );
  void * n1 = h0.insert( 3, 1 );
  void * n2 = h0.insert( -1, 2 );
  void * n3 = h0.insert( 10, 3 );
  void * n4 = h0.insert( 11, 4 );
  void * n5 = h0.insert( 12, 5 );
  void * n6 = h0.insert( 13, 6 );
  void * n7 = h0.insert( 14, 7 );
  void * n8 = h0.insert( 15, 8 );
  void * n9 = h0.insert( 16, 9 );
  
  CHECK( h0.get_num_nodes() == 10 );
  CHECK( h0.get_num_root_trees() == 10 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 10 );

  //force tree reconstruction by extracting a minimum
  //{ n1->{ n0, n3->{n4}, n5->{ n6, n7->{n8} } }, n9 }
  int key, item;
  void * invalidated;
  bool ret = h0.extract_min( key, item, invalidated );
  CHECK( ret );
  CHECK( -1 == key );
  CHECK( 2 == item );
  CHECK( invalidated == n2 );
  CHECK( h0.get_num_nodes() == 9 );
  CHECK( h0.get_num_root_trees() == 2 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 2 );
  
  //force marking nodes by decreasing key
  h0.decrease_key( n4, -2 );
  //{ n1->{ n0, n3(marked), n5->{ n6, n7->{n8} } }, n9, n4 }
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( -2 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 4 == item );
  CHECK( h0.get_num_nodes() == 9 );
  CHECK( h0.get_num_root_trees() == 3 );
  CHECK( h0.get_num_marked_nodes() == 1 );
  CHECK( h0.get_potential() == 3 + 2 );
 

  h0.decrease_key( n7, -3 );
  //{ n1->{ n0, n3(marked), n5(marked)->{ n6 } }, n9, n4, n7->{n8} }
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( -3 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 7 == item );
  CHECK( h0.get_num_nodes() == 9 );
  CHECK( h0.get_num_root_trees() == 4 );
  CHECK( h0.get_num_marked_nodes() == 2 );
  CHECK( h0.get_potential() == 4 + 2*2 );

  h0.decrease_key( n6, -4 );
  //{ n1->{ n0, n3(marked) }, n9, n4, n7->{n8}, n6, n5 }
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( -4 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 6 == item );
  CHECK( h0.get_num_nodes() == 9 );
  CHECK( h0.get_num_root_trees() == 6 );
  CHECK( h0.get_num_marked_nodes() == 1 );
  CHECK( h0.get_potential() == 6 + 1*2 );
  CHECK( (((heap_fib<int>::node*)n3)->_marked == true) );

  h0.decrease_key( n3, -5 );
  //{ n1->{ n0 }, n9, n4, n7->{n8}, n6, n5, n3 }
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( -5 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 3 == item );
  CHECK( h0.get_num_nodes() == 9 );
  CHECK( h0.get_num_root_trees() == 7 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 7 );

  h0.decrease_key( n7, -6 );
  //{ n1->{ n0 }, n9, n4, n7->{n8}, n6, n5, n3 }
  ret = h0.min_key( key );
  CHECK( ret );
  CHECK( -6 == key );
  ret = h0.min_item( item );
  CHECK( ret );
  CHECK( 7 == item );
  CHECK( h0.get_num_nodes() == 9 );
  CHECK( h0.get_num_root_trees() == 7 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 7 );

  ret = h0.extract_min( key, item, invalidated );
  //{ n1->{ n0 }, n9, n4, n6, n5, n3, n8 }
  //{ { {}, { {} }, { {}, { {} } } } }
  CHECK( ret );
  CHECK( h0.get_num_nodes() == 8 );
  CHECK( h0.get_num_root_trees() == 1 );
  CHECK( h0.get_num_marked_nodes() == 0 );
  CHECK( h0.get_potential() == 1 );

  for( int i = 0; i < 8; ++i ){
      ret = h0.extract_min( key, item, invalidated );
      CHECK( ret );
      CHECK( h0.get_num_nodes() == 7 - i ); 
  }

}

TEST_CASE( "heap_fib stress", "[heap_fib]" ) {
  heap_fib<int> h0;
  default_random_engine gen;
  uniform_int_distribution<int> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max() );
  vector<int> keys{};
  vector<void*> handles{};
  for( int i = 0; i < 10000; ++i ){
    int n = distr(gen);
    if( n % 1 == 0 ){
      n = distr(gen);
      keys.push_back(n);
      void * h = h0.insert( n, i );
      handles.push_back( h );
    }else if( handles.size() > 0 ){
      n = distr(gen);
      n = n % handles.size();
      void * to_remove = handles[n];
      h0.remove( to_remove );
      auto it = handles.begin() + n;
      handles.erase( it );
      auto it_k = keys.begin() + n;
      keys.erase( it_k );
    }
  }
  CHECK( h0.get_num_nodes() == keys.size() );
  std::sort( keys.begin(), keys.end() );
  for( int i = 0; i < keys.size(); ++i ){
    int key, item;
    void * invalidated;
    bool ret = h0.extract_min( key, item, invalidated );
    CHECK( ret );
    CHECK( keys[i] == key );
  }
}
