#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "Clock.h"

TEST_CASE( "Clock", "[Clock]" ) { 

    Clock _clock;

    SECTION( "Clock Start Stop" ) {

        std::cout << "start clock\n";
        Clock _clock;
  
        bool bRet;

        bRet = _clock.SetFps(30);
        CHECK( bRet );
        
        _clock.Run();  

        std::cout << "wait for 2 seconds\n";
        std::this_thread::sleep_for (std::chrono::seconds(2));
        std::cout << "check clock time" <<endl;
 
        _clock.Tick();
        CHECK( bRet );

        CHECK( true == _clock.IsRunning() );

        _clock.Pause();
        
        CHECK( false == _clock.IsRunning() );

        double time = _clock.GetTime();
        cout << time << endl;
 
        CHECK( time >= 2000 );
        CHECK( time <= 2050 );

    }
}
