//scheduler implementation using queue_lockfree_total( unbounded lockfree total queue ) containing Funwrap3 elements

#include "IScheduler.hpp"
#include "Funwrap3.hpp"
#include "Thread0.hpp"
#include "queue_lockfree_total.hpp"

#include <deque>
#include <thread>
#include <mutex>
#include <list>
#include <map>

class Scheduler0 : public IScheduler< Funwrap3 > {
public:
    char const * get_id(){ return "scheduler0"; }
    using TaskType = Funwrap3;
    Scheduler0();
    ~Scheduler0();
    void reset() override;
    std::pair<bool, uint64_t> add_task( e_scheduler_priority, TaskType & item ) override;
    std::pair<bool, void*> query_task_complete( uint64_t task_id ) override;
    std::pair<bool, void*> set_threads( std::list<e_scheduler_priority> & p ) override;
    std::pair<bool, void*> set_pools( std::list<e_scheduler_priority> & p ) override;
    std::pair<bool, void*> run() override;
    std::pair<bool, void*> stop() override;
    size_t get_num_tasks() const override;
private:
    std::vector< Thread0 * > _threads;
    std::vector< queue_lockfree_total<Funwrap3> * > _pools;
    std::map< e_scheduler_priority, std::vector<size_t> > _pool_priorities;
    std::map< e_scheduler_priority, size_t > _pool_priority_index;
    uint64_t _task_id_generator;
    std::map< uint64_t, void * > _completion_storage;
    
};
