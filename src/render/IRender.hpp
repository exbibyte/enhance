#ifndef IRENDER_H
#define IRENDER_H

class IRender {
public:
    virtual ~IRender(){}
    bool render( RenderData );
};

#endif
