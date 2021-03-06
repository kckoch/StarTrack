#ifndef BULLET__H
#define BULLET__H
#include <string>
#include <vector>
#include "sprite.h"
#include "collisionStrategy.h"

class Bullet : public Sprite {
public:
    Bullet(const std::string& name);
    Bullet(const std::string& name, const Vector2f pos, const Vector2f vel);
    Bullet(const Bullet& b);
    virtual ~Bullet();
    virtual const Frame* getFrame() const { return frame; }
    virtual void draw() const;
    virtual void update(Uint32 ticks);
    void inUse(const Vector2f pos) {
        avail = false;
        startPos = pos;
        setPosition(pos);
    }
    void setVelo(int x, int y) {
        Vector2f velo = getVelocity();
        setVelocity(Vector2f(abs(velo[0])*x, abs(velo[1])*y));
    }
    bool checkDone() { return avail; }
    bool collidedWith(const Drawable* d) const {
        return strategy->execute(*this, *d);
    }
private:
    bool avail;
    int travelDist;
    Vector2f startPos;
    int xspeed;
    int yspeed;
    const Frame * frame;
    int frameWidth;
    int frameHeight;
    int worldWidth;
    int worldHeight;
    std::vector<CollisionStrategy*> strategies;
    CollisionStrategy *strategy;
    
    Bullet& operator=(const Bullet& s);
};
#endif
