#ifndef EN_TPCOMMON_H
#define EN_TPCOMMON_H

#include "enThreadPool.h"
#include "ThreadPool.h"
#include "BufferPool.h"
#include "CircularBufferThreadSafe.h"
#include "enThread.h"
#include "FuncWrap.h"

class enBufferPoolCommon : public BufferPool< CircularBufferThreadSafe< FuncWrap >, FuncWrap > {};
class enTPCommon : public enThreadPool< enBufferPoolCommon, enThread  >{};

#endif
