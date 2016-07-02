#ifndef WORKERPOOL0_H
#define WORKERPOOL0_H

#include "IWorkerpool.hpp"
#include "IThread.hpp"
#include <vector>

class Workerpool0 : public IWorkerpool {
public:
    ~Workerpool0();
    void set_worker_num( unsigned int );
    void set_worker_task( std::function<void(void)> );
    void start_workers();
    void end_workers();
private:
    std::vector<IThread*> _pool;
};

#endif
