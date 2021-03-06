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
    LIGHTGREY(SDL_MapRGB(screen->format, 0xdf, 0xdf, 0xdf)),
    show(true),
    god(false),
    username(Gamedata::getInstance().getXmlStr("username")),
    title(Gamedata::getInstance().getXmlStr("screenTitle")),
    frameMax(Gamedata::getInstance().getXmlInt("frameMax")),
    health(100),
    red(100),
    blue(100),
    yell(100) {}

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
        Draw_AALine(screen, 730, 80, 730, 10, 25, 211, 211, 211, 50);
        Draw_AALine(screen, 760, 80, 760, 10, 25, 211, 211, 211, 50);
        Draw_AALine(screen, 790, 80, 790, 10, 25, 211, 211, 211, 50);
        Draw_AALine(screen, 820, 80, 820, 10, 25, 211, 211, 211, 50);
        Draw_AALine(screen, 730, 80, 730, 80-(health*70)/100, 25, 211, 211, 211, 255);
        Draw_AALine(screen, 760, 80, 760, 80-(red*70)/100, 25, 211, 0, 0, 255);
        Draw_AALine(screen, 790, 80, 790, 80-(yell*70)/100, 25, 211, 211, 0, 255);
        Draw_AALine(screen, 820, 80, 820, 80-(blue*70)/100, 25, 0, 0, 211, 255);
        if(god) {
            io.printStringAfterMessage("God Mode: On", 10, 160);
        }
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
