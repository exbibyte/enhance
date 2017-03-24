#ifndef EN_ENGINE_KERNEL_1_H
#define EN_ENGINE_KERNEL_1_H

#include "enEngineKernelAbstract.hpp"

#include <map>
#include <functional>
#include <cstdint>
#include <vector>

//interface for core engine
class enEngineKernel1 : public enEngineKernelAbstract {
public:
    void init();
    void deinit();
};

#endif
