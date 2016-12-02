#ifndef SMARTROCK__H
#define SMARTROCK__H
#include <iostream>
#include "sprite.h"
#include "ioManager.h"
#include "gamedata.h"

class ExplodingSprite;

class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, int w, int h);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  void respawn();
  void explode();
  void takeHit();
  bool checkExplosion();

  void incrSafeDistance() {++safeDistance; }
  void decrSafeDistance() { --safeDistance; }
  int getSafeDistance() { return safeDistance; }
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

private:	
  enum MODE {NORMAL, EVADE};
  IOManager& io;	
  ExplodingSprite *explosion;
  int size;
  int health;
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  int worldWidth;
  int worldHeight;
  int safeDistance;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};
#endif
