#ifdef THREAD_H

using namespace std;

void Thread::WaitForThread(){
    while( _access.load(std::memory_order_relaxed) >= 0 );
}

bool Thread::Run(){
  _thread = std::thread( &Thread::Task, this );
  _thread.detach(); //let the thread loose
  return true;
}

void Thread::Task(){
  cout<<"Default Task Ran"<<endl;
  // keep consuming from buffer if buffer is not empty
  while( _globalpool && _globalpool->GetSize() > 0){ 
    FuncWrap nextItem;
    GetNextTask( nextItem ); // get item from buffer
    nextItem(); //execute task here
  }
}

void Thread::SetGlobalpool( Threadpool * b ){
  _globalpool = b;
}

#endif
