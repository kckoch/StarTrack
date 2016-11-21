#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "sprite.h"
#include "drawable.h"
#include "collisionStrategy.h"

class ExplodingSprite;

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player();

  virtual void draw() const;
  void updateMovement(int movein);
  virtual void update(Uint32 ticks);
  bool collidedWith(const Drawable* d) const {
    return strategy->execute(*this, *d);
  }
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  unsigned getCurrentFrame() const { return currentFrame; }
  void takeDamage(int size);
  int getHealth() const { return health; }
  virtual void respawn();
  virtual void explode();

private:
  ExplodingSprite *explosion;
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;
  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy *strategy;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  int xspeed;
  int yspeed;
  int health;
  int damage;
  
  int movement;
  Player& operator=(const Player&);
  void advanceFrame(Uint32 ticks);
};
#endif
