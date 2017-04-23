#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include <cassert>
#include <cstdlib>

#include "stress_hashtable.hpp"
#include "hashtable_lock_striped.hpp"

using namespace std;

int main(){

    e2::dsc::hashtable_lock_striped< int, int > hashtable( 200, 0.33 ); //200 buckets, 0.33 lock factor
    unsigned int num_threads = 1000;
    
    stress_hashtable::stress_put_get_int( num_threads, hashtable );

    return 0;
}
