#include "hud.h"
#include "aaline.h"
#include "gamedata.h"

Hud& Hud::getInstance() {
  static Hud hud;
  return hud;
}

Hud::Hud() : 
    io(IOManager::getInstance()),
    screen(io.getScreen()),
    clock(Clock::getInstance()),
    GREY(SDL_MapRGB(screen->format, 0xcd, 0xcd, 0xcd)),
    show(true),
    god(false),
    username(Gamedata::getInstance().getXmlStr("username")),
    title(Gamedata::getInstance().getXmlStr("screenTitle")),
    frameMax(Gamedata::getInstance().getXmlInt("frameMax")),
    health(100) {}

void Hud::draw() const {
    if(show) {
        int x = 120;
        int y = 200;
        io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 20);
        io.printMessageValueAt("FPS: ", clock.getFPS(), 10, 40);
        io.printStringAfterMessage("Up: W", 10, 60);
        io.printStringAfterMessage("Left: A", 10, 80);
        io.printStringAfterMessage("Down: S", 10, 100);
        io.printStringAfterMessage("Right: D", 10, 120);
        io.printStringAfterMessage("Toggle HUD: F1", 10, 140);
        io.printMessageAt(title, 10, 450);
        Draw_AALine(screen, 0, 0, x, 0, GREY);
        Draw_AALine(screen, 0, 0, 0, y, GREY);
        Draw_AALine(screen, 0, y, x, y, GREY);
        Draw_AALine(screen, x, 0, x, y, GREY);
        if(god) {
            io.printStringAfterMessage("God Mode: On", 10, 160);
        }
        io.printMessageValueAt("Health:", health, 320, 60);
    }
}

void Hud::setShow(bool in) {
    show = in;
}

void Hud::toggle(){
    show = !show;
}

void Hud::toggleGod() {
    god = !god;
}
