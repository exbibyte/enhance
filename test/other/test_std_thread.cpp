#include <thread>
#include <iostream>

using namespace std;

void task( int * start, int offset, int val, int repeat ){
  for( int i = 0; i < repeat; i++ ){
    start[offset + i] = val;
  }
}

int main(){

  int array[10];
  int * array_p = array;

  thread a(task, array_p, 1, 7, 4);
  thread b(task, array_p, 5, 11, 5);
 
  a.join();
  b.join();

  cout<<"num hardware threads: "<< thread::hardware_concurrency()<< endl;

  for( int i = 0; i < 10; i++ ){
    cout<< array[i] <<" ";
  }
  cout<<endl;

  return 0;
}
