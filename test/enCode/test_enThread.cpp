#include "enThread.h"

#include <string>
#include <iostream>
using namespace std;

int main(){

  enThread t;
  string name = "t1";
  ThreadPriority priori = THREAD_PRIORITY_NORMAL;
  ThreadStacksize stacksize = THREAD_STACKSIZE_NORMAL;
  

  cout<<"IsBusy: "<< t.IsBusy() <<endl;
  cout<<"GetAcquirer: "<< t.GetAcquirer() <<endl;
  int processID = 0;
  t.Acquire( processID );
  cout<<"GetAcquirer: "<< t.GetAcquirer() <<endl;
  t.SetThread( name.c_str(), priori, stacksize ); 
  cout<<"thread name: "<< t._name<<endl;
  t.Run();
  cout<<"GetAcquirer: "<< t.GetAcquirer() <<endl;

  return 0;
}
