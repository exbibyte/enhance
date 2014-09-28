#ifdef THREAD_H

void Thread::WaitForThread(){
    while( _access.load(std::memory_order_relaxed) >= 0 );
}

bool Thread::Run(){
  _StdThread = std::thread( &Thread::RunHook, this );
  _StdThread.detach(); //let the thread loose
  return true;
}

void Thread::RunHook(){
}

#endif
