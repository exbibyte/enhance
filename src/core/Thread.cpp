#ifdef THREAD_H

void Thread::WaitForThreadEnd(){   /// blocking until end state
    while( _Access.load(std::memory_order_relaxed) != THREAD_ENDED );
}

void Thread::WaitForThreadIdle(){   /// blocking until idle state
    while( _Access.load(std::memory_order_relaxed) != THREAD_IDLE );
}

void Thread::WaitForThreadNotPause(){   /// blocking until not pausing state
    while( _Access.load(std::memory_order_relaxed) == THREAD_PAUSED );
}

bool Thread::Run(){
    _Access.store( THREAD_BUSY, std::memory_order_relaxed); /// start thread and flag busy state
    _StdThread = std::thread( &Thread::RunHook, this );
    _StdThread.detach(); //let the thread loose
    return true;
}

void Thread::RunHook(){
    _Access.store( THREAD_ENDED, std::memory_order_relaxed); /// default action is end thread
}

void Thread::PauseThread(){
    if( _Notify.load(std::memory_order_release) != THREAD_PAUSED ){ /// flag pause state if current state is not paused
        _Notify.store( THREAD_NOTIFY_PAUSE, std::memory_order_relaxed);
    }
}

void Thread::EndThread(){
    if( _Notify.load(std::memory_order_release) != THREAD_ENDED ){ /// flag end state if current state is not ended
        _Notify.store( THREAD_NOTIFY_END, std::memory_order_relaxed);
    }
}

void Thread::ContinueThread(){
    if( _Notify.load(std::memory_order_relaxed) == THREAD_PAUSED ){ /// flag continue state if current state is paused
        _Notify.store( THREAD_NOTIFY_CONTINUE, std::memory_order_relaxed);
    }
}

#endif
