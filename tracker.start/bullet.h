#ifndef BULLET__H
#define BULLET__H
#include <string>
#include "sprite.h"

class Bullet : public Sprite {
public:
    Bullet(const std::string& name);
    Bullet(const std::string& name, const Vector2f pos, const Vector2f vel);
    Bullet(const Bullet& b);
    virtual ~Bullet() { } 
    virtual const Frame* getFrame() const { return frame; }
    //virtual void draw() const;
    virtual void update(Uint32 ticks);
    void inUse(const Vector2f pos) {
        avail = false;
        startPos = pos;
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
    
    Bullet& operator=(const Bullet& s);
};
#endif