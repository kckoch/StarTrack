#include <cmath>
#include <stdlib.h>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

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
  size(Gamedata::getInstance().getXmlInt(name+"/size"))
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
  size(Gamedata::getInstance().getXmlInt(name+"/size"))
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
  size(Gamedata::getInstance().getXmlInt(name +"/size"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  explosion(s.explosion),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  size(s.size)
{ }

Sprite::~Sprite() {
  delete explosion;
}

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
  return *this;
}

void Sprite::respawn() {
  X(rand()%Gamedata::getInstance().getXmlInt("maxWidth"));
  Y(rand()%worldHeight);
  std::cout << "Respawning: " << X() << " " << Y() << std::endl;
}

void Sprite::explode() {
  if(explosion) return;
  explosion = new ExplodingSprite(*this);
  respawn();
}
