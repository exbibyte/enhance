#ifndef EN_ENGINE_E0_H
#define EN_ENGINE_E0_H

#include "enEngine.h"
#include "enLog0.h"
#include "enCore0.h"
#include "enUI0.h"
#include "enFrontend0.h"
#include "enBackend0.h"
#include "enRenderPass.h"
#include "enMain0.h"

class enEngineE0 : public enEngine< enLog0, enCore0, enUI0, enFrontend0, enBackend0, enRenderPass_ShadowMap_OpGL, enMain0 > {
    bool Initialize() override {
	return true;
    }
    bool Run() override {
	return true;
    }
    bool Cleanup() override {
	return true;
    }
};

#endif
