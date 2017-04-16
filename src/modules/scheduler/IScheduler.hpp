#ifndef ISCHEDULER_H
#define ISCHEDULER_H

#include <cstring>
#include <cstdint>
#include <list>
#include <vector>

enum class e_scheduler_priority {
    low,
    medium,
    high,
};

template< class TaskType >
class IScheduler {
public:
    virtual ~IScheduler(){}
    virtual void reset() {}
    virtual std::pair<bool, uint64_t/*task_id*/> add_task( e_scheduler_priority, TaskType & ){ return { false, 0 }; }
    virtual std::pair<bool, void*> query_task_complete( uint64_t task_id ){ return { false, 0 }; } //todo
    virtual std::pair<bool, void*> set_threads( std::list<e_scheduler_priority> & p ){ return { false, 0 }; } //to be called after set_pools
    virtual std::pair<bool, void*> set_pools( std::list<e_scheduler_priority> & p ){ return { false, 0 }; }
    virtual std::pair<bool, void*> run(){ return { false, 0 }; }
    virtual std::pair<bool, void*> stop(){ return { false, 0 }; }
    virtual size_t get_num_tasks() const { return 0; }
};

#endif
