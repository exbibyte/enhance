#include "ThreadPool.h"

#include <deque>
#include <iostream>
#include <future>
#include <thread>

using namespace std;

void test(){
  cout<<"task called"<<endl;
}

void test2(int a){
  cout<<"task2 called: "<< "argument: " << a <<endl;
}

int main(){

  ThreadPool tp;

  int a = 7;

  std::future<void> ret = tp.Submit(test);
  std::future<void> ret2 = tp.Submit(test2, 5);
  
  FuncWrap fw = std::move(tp._queue.back());
  fw();
  tp._queue.pop_back();
  FuncWrap fw2 = std::move(tp._queue.back());
  fw2();
  tp._queue.pop_back();

  return 0;
}
