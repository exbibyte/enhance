#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "BufferPool.h"
#include "QueueLF.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

TEST_CASE( "ThreadPool", "[ThreadPool]" ) {

    BufferPool< QueueLF, string > buffer;

    SECTION( "Add and get from buffer" ) {
        bool bRet;
        string str1 = "quartet";
        string str2 = "voyageur";
        bRet = buffer.AddToBuffer( str1 );
	CHECK( bRet );
	bRet = buffer.AddToBuffer( str2 );
	CHECK( bRet );
	string str_retrieve;
	bRet = buffer.GetFromBuffer( str_retrieve );
	CHECK( bRet );
	CHECK( str_retrieve == str1 );
	bRet = buffer.GetFromBuffer( str_retrieve );
	CHECK( bRet );
	CHECK( str_retrieve == str2 );
	//empty buffer retrieve
	bRet = buffer.GetFromBuffer( str_retrieve );
	CHECK( !bRet );
    }
}
