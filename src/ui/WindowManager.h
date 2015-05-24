#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <map>
#include <functional>
#include <bitset>

enum class KeyButtonWhich {
    MOUSE_L = 0,
    MOUSE_R,
    MOUSE_M,
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_SPC,	
    KEY_J,
    KEY_K,
    KEY_L
};

enum class KeyButtonState {
    Up = 0,
    DOWN    
};

class WindowManager{
public:
    WindowManager();
    virtual bool CreateWindow( int & iId, unsigned int ui_width, unsigned int ui_height, string const strTitle )=0;
    virtual bool SetSize( int iId, unsigned int ui_width, unsigned int ui_height ) = 0;
    virtual bool GetSize( int iId, unsigned int & ui_width, unsigned int & ui_height ) = 0;
    virtual bool SetFullScreen( int iId, bool bFullScreen ) = 0;
    virtual bool CloseWindow( int iId ) = 0;
    virtual bool GetCursorPos( int iId, double & xpos, double & ypos ) = 0;
    virtual bool GetCursorState( int iId, KeyButtonWhich which, KeyButtonState & state ) = 0;
    virtual bool SetKeyCombCallback( int iId, std::map<KeyButtonWhich, KeyButtonState> combo, std::function<int(int,int)> cb ) = 0;
    virtual bool ProcessKeyButtonCombo( int iId ) = 0;

private:
    std::map<std::string, std::function<bool(void)> cb> _MapKeyButtonComboCb;
    std::bitset<32> _KeyButtonComboCurrent;
};

#endif
