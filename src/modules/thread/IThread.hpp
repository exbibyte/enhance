#ifndef ITHREAD_H
#define ITHREAD_H

#include <functional>

class IThread {
public:
    enum class State {
	STOPPED,
	BUSY,
	SIGNAL_TO_STOP,
	SIGNAL_TO_BUSY,
    };
    enum class Action {
	START,
	END,
    };
    virtual ~IThread(){}
    virtual bool setaction( IThread::Action ) = 0;
    virtual IThread::State getstate() const = 0;
    virtual void settask( std::function<void(int)> ) = 0;
};

#endif
