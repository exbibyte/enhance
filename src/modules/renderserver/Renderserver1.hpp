#ifndef RENDERSERVER1_H
#define RENDERSERVER1_H

#include "IRenderserver.hpp"

#include <memory>

class GLSLProgram;

class Renderserver1 : public IRenderserver {
public:
    char const * get_id(){ return "renderserver1"; }
    bool init() override;
    virtual bool setdata(){ return false; }
    bool deinit() override;
// private:
    std::shared_ptr<GLSLProgram> _glslprogram;
};

#endif
