#ifndef IWORKERPOOL_H
#define IWORKERPOOL_H

class IWorkerpool {
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
    virtual void set_worker_num( unsigned int ){}
    virtual void set_action( IWorkerpool::Action ){}
    virtual IWorkerpool::Status get_status() const { return _status; }
private:
    IWorkerpool::Status _status;
};

#endif
