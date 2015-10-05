#ifndef STREAM_MANAGER_H
#define STREAM_MANAGER_H

#include <map>

enum class StreamType { GRAPHICS, LOGIC, UI };

class StreamChannel;

class StreamManager {
public:
    StreamManager();
    ~StreamManager();
    bool SelectStream( StreamType, StreamChannel * & stream );
private:
    std::map< StreamType, StreamChannel * > _MapStreamChannel;
};

#endif
