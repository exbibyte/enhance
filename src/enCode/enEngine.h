#ifndef EN_ENGINE_H
#define EN_ENGINE_H

template< class CoreEngine, class LogicEngine, class ContentEngine, class WindowEngine, class RenderEngine, class UiEngine, class FrontEngine, class NetworkEngine, class LogEngine >
class enEngine {
public:
    using TypeCoreEngine = CoreEngine;
    using TypeLogicEngine = LogicEngine;
    using TypeContentEngine = ContentEngine;
    using TypeWindowEngine = WindowEngine;
    using TypeRenderEngine = RenderEngine;
    using TypeUiEngine = UiEngine;
    using TypeFrontEngine = FrontEngine;
    using TypeNetworkEngine = NetworkEngine;
    using TypeLogEngine = LogEngine;
    CoreEngine _core_engine;
    ContentEngine _content_engine;
    WindowEngine _window_engine;
    RenderEngine _render_engine;
    UiEngine _ui_engine;
    FrontEngine _front_engine;
    NetworkEngine _network_engine;
    LogEngine _log_engine;
    bool Initialize();
    bool Run();
    bool Cleanup();
};

template< class CoreEngine, class LogicEngine, class ContentEngine, class WindowEngine, class RenderEngine, class UiEngine, class FrontEngine, class NetworkEngine, class LogEngine >
bool enEngine< CoreEngine, LogicEngine, ContentEngine, WindowEngine, RenderEngine, UiEngine, FrontEngine, NetworkEngine, class LogEngine >::Initialize {
    //TODO
    return true;
}
template< class CoreEngine, class LogicEngine, class ContentEngine, class WindowEngine, class RenderEngine, class UiEngine, class FrontEngine, class NetworkEngine, class LogEngine >
bool enEngine< CoreEngine, LogicEngine, ContentEngine, WindowEngine, RenderEngine, UiEngine, FrontEngine, NetworkEngine, class LogEngine >::Run {
    //TODO
    return true;
}
template< class CoreEngine, class LogicEngine, class ContentEngine, class WindowEngine, class RenderEngine, class UiEngine, class FrontEngine, class NetworkEngine, class LogEngine >
bool enEngine< CoreEngine, LogicEngine, ContentEngine, WindowEngine, RenderEngine, UiEngine, FrontEngine, NetworkEngine, class LogEngine >::Cleanup {
    //TODO
    return true;
}

#endif
