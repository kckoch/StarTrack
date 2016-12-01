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
    void setHealth(int healthin) { health = healthin; }
    void setRed(int redin) { red = redin; }
    void setBlue(int bluein) { blue = bluein; }
    void setYellow(int yellin) { yell = yellin; }
    void setFree(int freein) { free = freein; }
    void setUse(int bullin) { bull = bullin; }
private:
    const IOManager& io;
    SDL_Surface * const screen;
    Clock& clock;
    const Uint32 GREY;
    const Uint32 LIGHTGREY;
    bool show;
    bool god;
    const std::string username;
    const std::string title;
    const int frameMax;
    int health;
    int red;
    int blue;
    int yell;
    int free;
    int bull;
    
    Hud();
    Hud(const Hud &h);
    Hud& operator=(const Hud& h);
};
#endif
