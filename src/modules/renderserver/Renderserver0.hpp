#ifndef RENDERSERVER0_H
#define RENDERSERVER0_H

#include "IRenderserver.hpp"

#include <memory>

class GLSLProgram;

class Renderserver0 : public IRenderserver {
public:
    char const * get_id(){ return "renderserver0"; }
    bool init() override;
    virtual bool setdata(){ return false; }
    bool deinit() override;
// private:
    std::shared_ptr<GLSLProgram> _glslprogram;
};

#endif
