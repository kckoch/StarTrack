#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

Player::~Player() {
  for (unsigned i = 0; i < strategies.size(); ++i) {
    delete strategies[i];
  }
  delete explosion;
}

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
	    if(velocityX() < 0) {
            currentFrame = 1;
		} else if (velocityX() > 0) {
		    currentFrame = 0;
		}
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(0,0)
           ),
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  elapsedTicks(0),
  worldWidth(WORLD_WIDTH),
  worldHeight(WORLD_HEIGHT),
  strategies(),
  strategy(NULL),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  xspeed(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  yspeed(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  health(Gamedata::getInstance().getXmlInt(name+"/health")),
  damage(Gamedata::getInstance().getXmlInt("damage")),
  red(Gamedata::getInstance().getXmlInt(name+"/red")),
  blue(Gamedata::getInstance().getXmlInt(name+"/yellow")),
  yellow(Gamedata::getInstance().getXmlInt(name+"/blue")),
  maxLevel(Gamedata::getInstance().getXmlInt(name+"/blue")),
  movement(0)
  {
      strategies.push_back( new MidPointCollisionStrategy );
      strategies.push_back( new RectangularCollisionStrategy );
      strategies.push_back( new PerPixelCollisionStrategy );
      strategy = strategies[Gamedata::getInstance().getXmlInt("strategy")];
  }

Player::Player(const Player& s) :
  Drawable(s),
  explosion(s.explosion),
  frames(s.frames),
  elapsedTicks(s.elapsedTicks),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  strategies(),
  strategy(s.strategy),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  xspeed(s.xspeed),
  yspeed(s.yspeed),
  health(s.health),
  damage(s.damage),
  red(s.red),
  blue(s.blue),
  yellow(s.yellow),
  maxLevel(s.maxLevel),
  movement(s.movement)
  {
      strategies.push_back( new MidPointCollisionStrategy );
      strategies.push_back( new RectangularCollisionStrategy );
      strategies.push_back( new PerPixelCollisionStrategy );
  }

void Player::draw() const {
  if(explosion) {
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
  strategy->draw();
}

void Player::updateMovement(int movement) {
    switch(movement) {
        case 1:
            velocityY(-yspeed);
            velocityX(0);
            break;
        case 2:
            velocityX(xspeed);
            velocityY(0);
            break;
        case 3:
            velocityY(-yspeed);
            velocityX(xspeed);
            break;
        case 4:
            velocityY(yspeed);
            velocityX(0);
            break;
        case 6:
            velocityY(yspeed);
            velocityX(xspeed);
            break;
        case 8:
            velocityX(-xspeed);
            velocityY(0);
            break;
        case 9:
            velocityY(-yspeed);
            velocityX(-xspeed);
            break;
        case 12:
            velocityY(yspeed);
            velocityX(-xspeed);
            break;
        default:
            velocityY(0);
            velocityX(0);
            break;
    }
}

void Player::update(Uint32 ticks) { 
    advanceFrame(ticks);
    
    elapsedTicks += ticks;
    if(elapsedTicks > 4000) {
        red -= 10;
        blue -= 10;
        yellow -= 10;
        elapsedTicks = 0;
        if(red <= 0 || blue <= 0 || yellow <= 0)
            explode();
    }
    
    if(explosion) {
        explosion->update(ticks);
        if(explosion->chunkCount()==0) {
            delete explosion;
            explosion = NULL;
            respawn();
        }
        return;
    }
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    
    if ( Y() < 0) {
        velocityY(0);
        Y(0);
    } else if ( Y() > worldHeight-frameHeight) {
        velocityY(0);
        Y(worldHeight-frameHeight);
    } else {
        setPosition(getPosition() + incr);
    }
    if ( X() < 0) {
        X(0);
        velocityX(0);
    } else {
        setPosition(getPosition() + incr);
    }
    
}

void Player::takeDamage(int size) {
    health -= damage*size;
    if(health <= 0)
        explode();
}

void Player::gainRed(int redin) {
    red += redin;
    if(red > maxLevel) red = maxLevel;
}

void Player::gainBlue(int bluein) {
    blue += bluein;
    if(blue > maxLevel) blue = maxLevel;
}

void Player::gainYellow(int yellin) {
    yellow += yellin;
    if(yellow > maxLevel) yellow = maxLevel;
}

void Player::respawn() {
    setPosition(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"), 
                         Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")));
    currentFrame = 0;
    timeSinceLastFrame = 0;
    health = Gamedata::getInstance().getXmlInt(getName()+"/health");
    red = Gamedata::getInstance().getXmlInt(getName()+"/red");
    blue = Gamedata::getInstance().getXmlInt(getName()+"/yellow");
    yellow = Gamedata::getInstance().getXmlInt(getName()+"/blue");
}

void Player::explode() {
    if(explosion) return;
    explosion = 
        new ExplodingSprite(Sprite(getName(), getPosition(), Vector2f(0,0), getFrame()));
}
