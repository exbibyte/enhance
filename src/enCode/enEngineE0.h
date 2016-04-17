#ifndef EN_ENGINE_E0_H
#define EN_ENGINE_E0_H

#include "enEngine.h"

class enEngineE0 : public enEngine< EngineCoreDefault, EngineLogicDefault, EngineContentDefault, EngineWindowDefault, EngineRenderDefault, EngineUiDefault, EngineFrontDefault, EngineNetworkDefault, EngineLogDefault > {
    bool Initialize(){
	return false;
    }
    bool Run(){
	return false;
    }
    bool Cleanup(){
	return false;
    }
};

#endif
