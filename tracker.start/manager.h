#include <vector>
#include <list>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "bullet.h"
#include "explodingSprite.h"
#include "hud.h"
#include "sound.h"
#include "smartSprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  SDLSound& sound;

  SDL_Surface * const screen;
  World space;
  World star1;
  World star2;
  World star3;
  Hud &hud;
  Viewport& viewport;

  Player *player;
  std::vector<Sprite*> sprites;
  std::vector<SmartSprite*> smarties;
  std::list<Bullet*> freeBull;
  std::list<Bullet*> inUse;
  std::vector<int> collSize;

  bool makeVideo;
  bool god;
  bool pool;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  const unsigned int hideAfter;

  void draw() const;
  void update();
  void checkForCollisions();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
