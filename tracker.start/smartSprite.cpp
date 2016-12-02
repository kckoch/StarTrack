#include <cmath>
#include "viewport.h"
#include "smartSprite.h"
#include "explodingSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name, int w, int h) :
 Sprite(name, Vector2f(3000, 30), 
   Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
            Gamedata::getInstance().getXmlInt(name+"/speedY"))
 ), 
 io(IOManager::getInstance()),
 explosion(NULL),
 size(Gamedata::getInstance().getXmlInt(name+"/size")),
 health(Gamedata::getInstance().getXmlInt(name+"/health")),
 playerPos(),
 playerWidth(w),
 playerHeight(h),
 worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
 worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
 safeDistance(Gamedata::getInstance().getXmlInt(name+"/safedist")),
 currentMode(NORMAL) 
{ 
    X(rand()%Gamedata::getInstance().getXmlInt("maxWidth"));
    Y(rand()%worldHeight);
}

void SmartSprite::goLeft()  { 
  if (X() > 0) velocityX( -abs(velocityX()) ); 
}
void SmartSprite::goRight() { velocityX( fabs(velocityX()) ); }
void SmartSprite::goUp()    { velocityY( -fabs(velocityY()) ); }
void SmartSprite::goDown()  { velocityY( fabs(velocityY()) ); }

void SmartSprite::draw() const {
  if(explosion) {
    explosion->draw();
    return;
  }
  Sprite::draw();
}

void SmartSprite::update(Uint32 ticks) {
  if(explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount()==0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Sprite::update(ticks);
  float x= X()+getFrame()->getWidth()/2;
  float y= Y()+getFrame()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }
}

void SmartSprite::respawn() {
  X(rand()%Gamedata::getInstance().getXmlInt("maxWidth"));
  Y(rand()%worldHeight);
}

void SmartSprite::explode() {
  if(explosion) return;
  explosion = new ExplodingSprite(*this);
  respawn();
}

bool SmartSprite::checkExplosion() {
  if(explosion) return true;
  return false;
}

void SmartSprite::takeHit() {
  health -= 1;
  if(health == 0)
    explode();
}

