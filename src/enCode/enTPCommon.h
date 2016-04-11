#ifndef EN_TPCOMMON_H
#define EN_TPCOMMON_H

#include "enThreadPool.h"
#include "ThreadPool.h"
#include "BufferPool.h"
#include "enThread.h"
#include "FuncWrap.h"
#include "QueueLF.h"

class enBufferPoolCommon : public BufferPool< QueueLF, FuncWrap > {};
class enTPCommon : public enThreadPool< enBufferPoolCommon, enThread  >{};

#endif
