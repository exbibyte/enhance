#ifndef EN_ENGINE_KERNEL_0_H
#define EN_ENGINE_KERNEL_0_H

#include "enEngineKernelAbstract.hpp"

#include <map>
#include <functional>
#include <cstdint>
#include <vector>

//interface for core engine
class enEngineKernel0 : public enEngineKernelAbstract {
    void Init();
    void Deinit();
};

#endif
