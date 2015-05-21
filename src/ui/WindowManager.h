#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

class WindowManager{
public:
    WindowManager();
    virtual bool CreateWindow( int & iId, unsigned int ui_width, unsigned int ui_height, string const strTitle ){ return false; }
    virtual bool SetSize( int iId, unsigned int ui_width, unsigned int ui_height ){ return false; }
    virtual bool GetSize( int iId, unsigned int & ui_width, unsigned int & ui_height ){ return false; }
    virtual bool SetFullScreen( int iId, bool bFullScreen ){ return false; }
    virtual bool CloseWindow( int iId ){ return false; }
};

#endif
