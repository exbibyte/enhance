#ifndef IWORKER_H
#define IWORKER_H

#include <atomic>

template< template<class> class SourceQueue, class WorkType >
class IWorker {
public:
    enum class Status {
	STOPPED,
	BUSY,
	PAUSED,
    };
    enum class Action {
        STOP,
	PAUSE,
	START,
    };
    virtual ~IWorker(){};
    IWorker() : _status(STOPPED) {};
    virtual void set_source_queue( SourceQueue<WorkType> * source_queue ){}
    virtual void set_action( IWorker::Action ){}
    virtual IWorker::Status get_status() const { return _status; }
private:
    IWorker::Status _status;
};

#endif
