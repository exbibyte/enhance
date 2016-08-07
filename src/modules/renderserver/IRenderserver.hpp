#ifndef IRENDERSERVER_H
#define IRENDERSERVER_H

class IRenderserver {
public:
    virtual ~IRenderserver(){}
    virtual bool init() = 0;
    virtual bool deinit() = 0;
};

#endif
