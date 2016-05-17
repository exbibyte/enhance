#ifndef EN_ENGINE_H
#define EN_ENGINE_H

//encapsulating engine declaration
template< class LogEngine, class CoreEngine, class UIEngine, class FrontendEngine, class BackendEngine, class RenderEngine, class MainEngine >
class enEngine {
public:
    using TypeEngineLog =      LogEngine;
    using TypeEngineCore =     CoreEngine;
    using TypeEngineUI =       UIEngine;
    using TypeFrontendEngine = FrontendEngine;
    using TypeEngineBackend =  BackendEngine;
    using TypeEngineRender =   RenderEngine;
    using TypeEngineMain =     MainEngine;
    LogEngine                  _log_engine;
    CoreEngine                 _core_engine;
    UIEngine                   _ui_engine;
    FrontendEngine             _frontend_engine;
    BackendEngine              _backend_engine;
    RenderEngine               _render_engine;
    MainEngine                 _main_engine;
    virtual bool               Initialize()       { return false; };
    virtual bool               Run()              { return false; };
    virtual bool               Cleanup()          { return false; };
};

#endif
