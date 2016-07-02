#ifndef IWORKERPOOL_H
#define IWORKERPOOL_H

#include <functional>

class IWorkerpool {
public:
    virtual ~IWorkerpool(){}
    virtual void set_worker_num( unsigned int ) = 0;
    virtual void set_worker_task( std::function<void(void)> ) = 0;
    virtual void start_workers() = 0;
    virtual void end_workers() = 0;
};

#endif
