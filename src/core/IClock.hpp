#ifndef ICLOCK_H
#define ICLOCK_H

#include <functional>

class IClock {
public:
    virtual                                          ~IClock(){}
    virtual void                                     run(){}
    virtual void                                     pause(){}
    virtual bool                                     is_running() const {return false;}
    virtual void                                     set_frequency(double frequency_Hz){}
    void                                             set_func_cb( std::function<void(void)> ){}
    bool                                             tick() {return false;}
};

#endif
