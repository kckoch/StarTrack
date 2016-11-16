#ifndef HUD_H
#define HUD_H
#include "ioManager.h"
#include "clock.h"

class Hud {
public:
    static Hud& getInstance();
    virtual void draw() const;
    void setShow(bool in);
    void toggle();
    void toggleGod();
private:
    const IOManager& io;
    SDL_Surface * const screen;
    Clock& clock;
    const Uint32 GREY;
    bool show;
    bool god;
    const std::string username;
    const std::string title;
    const int frameMax;
    
    Hud();
    Hud(const Hud &h);
    Hud& operator=(const Hud& h);
};
#endif