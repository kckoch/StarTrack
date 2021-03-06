#include <math.h>
#include "bullet.h"
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"

Bullet::~Bullet() {
    for (unsigned i = 0; i < strategies.size(); ++i) {
        delete strategies[i];
    }
}

Bullet::Bullet(const std::string& name) :
        Sprite(name, Vector2f(0,0), Vector2f(
            Gamedata::getInstance().getXmlInt("bullet/speedX"), 
            Gamedata::getInstance().getXmlInt("bullet/speedY"))),
        avail(true),
        travelDist(Gamedata::getInstance().getXmlInt("bullet/distance")),
        startPos(Vector2f(0,0)),
        xspeed(Gamedata::getInstance().getXmlInt("bullet/speedX")),
        yspeed(Gamedata::getInstance().getXmlInt("bullet/speedY")),
        frame(FrameFactory::getInstance().getFrame(name)),
        frameWidth(frame->getWidth()),
        frameHeight(frame->getHeight()),
        worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
        worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
        strategies(),
        strategy(NULL)
{
    strategies.push_back( new MidPointCollisionStrategy );
    strategies.push_back( new RectangularCollisionStrategy );
    strategies.push_back( new PerPixelCollisionStrategy );
    strategy = strategies[Gamedata::getInstance().getXmlInt("strategy")];
}

Bullet::Bullet(const std::string& name, const Vector2f pos, const Vector2f vel) :
        Sprite(name, pos, vel),
        avail(true),
        travelDist(Gamedata::getInstance().getXmlInt("bullet/distance")),
        startPos(pos),
        xspeed(Gamedata::getInstance().getXmlInt("bullet/speedX")),
        yspeed(Gamedata::getInstance().getXmlInt("bullet/speedY")),
        frame(FrameFactory::getInstance().getFrame(name)),
        frameWidth(frame->getWidth()),
        frameHeight(frame->getHeight()),
        worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
        worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
        strategies(),
        strategy(NULL)
{
    strategies.push_back( new MidPointCollisionStrategy );
    strategies.push_back( new RectangularCollisionStrategy );
    strategies.push_back( new PerPixelCollisionStrategy );
    strategy = strategies[Gamedata::getInstance().getXmlInt("strategy")];
}

Bullet::Bullet(const Bullet& b) :
    Sprite(b),
    avail(b.avail),
    travelDist(b.travelDist),
    startPos(b.startPos),
    xspeed(b.xspeed),
    yspeed(b.yspeed),
    frame(b.frame),
    frameWidth(b.getFrame()->getWidth()),
    frameHeight(b.getFrame()->getHeight()),
    worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
    worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
    strategies(b.strategies),
    strategy(b.strategy)
{ }

void Bullet::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

void Bullet::update(Uint32 ticks) {
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    Vector2f pos = getPosition();
    setPosition(pos + incr);
    if(abs(pos[0]-startPos[0]) >= travelDist || abs(pos[1]-startPos[1]) >= travelDist || 
            hypot(abs(pos[0]-startPos[0]), abs(pos[1]-startPos[1])) >= travelDist) {
        avail = true;
    }
}
