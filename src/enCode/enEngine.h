#ifndef EN_ENGINE_H
#define EN_ENGINE_H

//encapsulating engine declaration
template< class LogEngine, class CoreEngine, class UIEngine, class FrontendEngine, class BackendEngine, class RenderEngine, class MainEngine >
class enEngine {
public:
    using TypeEngineLog =      LogEngine;
    using TypeEngineCore =     CoreEngine;
    using TypeEngineUI =       UIEngine;
    using TypeEngineFrontend = FrontendEngine;
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
    bool                       initialize();
    virtual bool               initialize_specialize() { return false; };
    bool                       run();
    virtual bool               run_specialize()        { return false; }
    bool                       cleanup();
    virtual bool               cleanup_specialize()    { return false; }
};

template< class LogEngine, class CoreEngine, class UIEngine, class FrontendEngine, class BackendEngine, class RenderEngine, class MainEngine >
bool enEngine< LogEngine, CoreEngine, UIEngine, FrontendEngine, BackendEngine, RenderEngine, MainEngine >::initialize(){
    _core_engine.RegisterComponent( TypeEngineLog::_component_type,  &_log_engine );
    _core_engine.RegisterComponent( TypeEngineCore::_component_type, &_core_engine );
    _core_engine.RegisterComponent( TypeEngineUI::_component_type,   &_ui_engine );
    _core_engine.RegisterComponent( TypeEngineFrontend::_component_type,  &_frontend_engine );
    _core_engine.RegisterComponent( TypeEngineBackend::_component_type,   &_backend_engine );
    _core_engine.RegisterComponent( TypeEngineRender::_component_type,    &_render_engine );
    _core_engine.RegisterComponent( TypeEngineMain::_component_type,      &_main_engine );

    return initialize_specialize();
};

template< class LogEngine, class CoreEngine, class UIEngine, class FrontendEngine, class BackendEngine, class RenderEngine, class MainEngine >
bool enEngine< LogEngine, CoreEngine, UIEngine, FrontendEngine, BackendEngine, RenderEngine, MainEngine >::run(){
    return run_specialize();
}

template< class LogEngine, class CoreEngine, class UIEngine, class FrontendEngine, class BackendEngine, class RenderEngine, class MainEngine >
bool enEngine< LogEngine, CoreEngine, UIEngine, FrontendEngine, BackendEngine, RenderEngine, MainEngine >::cleanup(){
    return cleanup_specialize();
}

#endif
