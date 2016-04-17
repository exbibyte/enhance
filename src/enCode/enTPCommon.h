#ifndef EN_TPCOMMON_H
#define EN_TPCOMMON_H

#include "enThreadPool.h"
#include "ThreadPool.h"
#include "BufferInterface.h"
#include "enThread.h"
#include "FuncWrap2.h"
#include "QueueLF.h"

class enThreadSpecialized : public enThread<ThreadPool2, FuncWrap2 >{};
class enBuffer_QueueLF_FuncWrap2 : public BufferInterface< QueueLF, FuncWrap2 > {};
class enTPCommon : public enThreadPool< enBuffer_QueueLF_FuncWrap2, enThreadSpecialized >{};

#endif
