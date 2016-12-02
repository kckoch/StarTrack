#include <cmath>
#include <stdlib.h>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

Sprite::~Sprite() {
    delete explosion;
}

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(3000, 140), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  explosion(NULL),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  size(Gamedata::getInstance().getXmlInt(name+"/size")),
  health(Gamedata::getInstance().getXmlInt(name+"/health"))
{
    X(rand()%Gamedata::getInstance().getXmlInt("maxWidth"));
    Y(rand()%worldHeight);
}

Sprite::Sprite(const string& name, const Vector2f& pos, const Vector2f& vel):
  Drawable(name, pos, vel), 
  explosion(NULL),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  size(Gamedata::getInstance().getXmlInt(name+"/size")),
  health(Gamedata::getInstance().getXmlInt(name+"/health"))
{ }

Sprite::Sprite(const string& name, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(name, pos, vel),
  explosion(NULL),
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  size(Gamedata::getInstance().getXmlInt(name +"/size")),
  health(Gamedata::getInstance().getXmlInt(name+"/health"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  explosion(s.explosion),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  size(s.size),
  health(s.health)
{ }

void Sprite::draw() const {
  if(explosion) {
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) {
  if(explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount()==0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < -100) {
    respawn();
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}

Sprite& Sprite::operator=(const Sprite& s){
  explosion = s.explosion;
  frame = s.frame;
  frameWidth = s.frameWidth;
  frameHeight = s.frameHeight;
  worldWidth = s.worldWidth;
  worldHeight = s.worldHeight;
  size = s.size;
  health = s.health;
  return *this;
}

void Sprite::respawn() {
  X(rand()%Gamedata::getInstance().getXmlInt("maxWidth"));
  Y(rand()%worldHeight);
  health = Gamedata::getInstance().getXmlInt(getName()+"/health");
}

void Sprite::explode() {
  if(explosion) return;
  explosion = new ExplodingSprite(*this);
  respawn();
}

bool Sprite::checkExplosion() {
  if(explosion) return true;
  return false;
}

void Sprite::takeHit() {
  health -= 1;
  if(health <= 0)
    explode();
}
