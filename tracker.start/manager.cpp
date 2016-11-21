#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "2waymultisprite.h"
#include "player.h"
#include "sprite.h"
#include "bullet.h"
#include "explodingSprite.h"
#include "gamedata.h"
#include "manager.h"
#include "hud.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  space("space", Gamedata::getInstance().getXmlInt("space/factor") ),
  star1("star1", Gamedata::getInstance().getXmlInt("star1/factor") ),
  star2("star2", Gamedata::getInstance().getXmlInt("star2/factor") ),
  star3("star3", Gamedata::getInstance().getXmlInt("star3/factor") ),
  hud(Hud::getInstance()),
  viewport( Viewport::getInstance() ),
  player(),
  sprites(),
  freeBull(),
  inUse(),
  collSize(),
  makeVideo( false ),
  god(false),
  frameCount( 0 ),
  username(Gamedata::getInstance().getXmlStr("username")),
  title(Gamedata::getInstance().getXmlStr("screenTitle")),
  frameMax(Gamedata::getInstance().getXmlInt("frameMax")),
  hideAfter(Gamedata::getInstance().getXmlInt("hideHudAfter"))
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  player = new Player("ship");
  hud.setHealth(player->getHealth());
  int maxRocks = Gamedata::getInstance().getXmlInt("maxRocks");
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("smallblue/rarity"); i++) {
    sprites.push_back(new Sprite("smallblue"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("smallred/rarity"); i++) {
    sprites.push_back(new Sprite("smallred"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("smallyell/rarity"); i++) {
    sprites.push_back(new Sprite("smallyell"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("medblue/rarity"); i++) {
    sprites.push_back(new Sprite("medblue"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("medred/rarity"); i++) {
    sprites.push_back(new Sprite("medred"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("medyell/rarity"); i++) {
    sprites.push_back(new Sprite("medyell"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("bigblue/rarity"); i++) {
    sprites.push_back(new Sprite("bigblue"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("bigred/rarity"); i++) {
    sprites.push_back(new Sprite("bigred"));
  }
  for(int i = 0; i < maxRocks/Gamedata::getInstance().getXmlInt("bigyell/rarity"); i++) {
    sprites.push_back(new Sprite("bigyell"));
  }
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("maxBullets"); i++) {
    freeBull.push_back(new Bullet("bullet"));
  }
  viewport.setObjectToTrack(player);
}

bool Manager::checkForCollisions() {
  std::vector<Sprite*>::iterator sprite = sprites.begin();
  while ( sprite != sprites.end() ) {
    if ( player->collidedWith(*sprite) ) {
        (*sprite)->explode();
        std::cout << "exploded" << std::endl;
        return true;
    }
    ++sprite;
  }
  return false;
}

void Manager::draw() const {
  space.draw();
  star1.draw();
  star2.draw();
  star3.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  //int i = 0;
  for(std::list<Bullet*>::const_iterator it = inUse.begin(); it != inUse.end(); it++) {
    (*it)->draw();
    //io.printMessageValueAt("X: ", (*it)->X(), 500, 20+i*20);
    //io.printMessageValueAt("Y: ", (*it)->Y(), 600, 20+i*20);
    //i++;
  }

  io.printMessageValueAt("FreeList: ", freeBull.size(), 320, 20);
  io.printMessageValueAt("InUse: ", inUse.size(), 320, 40);
  //io.printMessageValueAt("X: ", player->X(), 320, 80);
  //io.printMessageValueAt("Y: ", player->Y(), 390, 80);
  player->draw();
  hud.draw();
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
  }
  player->update(ticks);
  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  for(std::list<Bullet*>::iterator it = inUse.begin(); it != inUse.end(); it++) {
    (*it)->update(ticks);
    if((*it)->checkDone()) {
      freeBull.push_back(*it);
      it = inUse.erase(it);
    }
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  if ( checkForCollisions() ) {
    hud.setHealth(player->getHealth());
  }
  if(clock.getSeconds() == hideAfter) {
    hud.setShow(false);
  }
  space.update();
  star1.update();
  star2.update();
  star3.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  int move;
  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if((keystate[SDLK_w] | keystate[SDLK_a]<<1 | keystate[SDLK_s]<<2 | keystate[SDLK_d]<<3) != move) {
            move = keystate[SDLK_w] | keystate[SDLK_a]<<1 | keystate[SDLK_s]<<2 | keystate[SDLK_d]<<3;
            player->updateMovement(move);
        }
        if(keystate[SDLK_F1]) {
          hud.toggle();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_g]) {
          hud.toggleGod();
          if(god) {
            god = false;
          } else {
            god = true;
          }
        }
        if (keystate[SDLK_SPACE]) {
          if(freeBull.size() < 2) {
            freeBull.push_back(new Bullet("bullet"));
            freeBull.push_back(new Bullet("bullet"));
          }
          int xVel = 0;
          int xPos = 0;
          int yPos = 23;
          int pVelX = player->velocityX();
          //player going to the left
          if(pVelX < 0) {
            xVel = -1;
            xPos = 2;
          } else if(pVelX > 0) {
            //player going to the right
            xVel = 1;
            xPos = 100;
          } else {
            //have to check which direction the ship is facing
            if(player->getCurrentFrame()==0){
              xVel = 1;
              xPos = 100;
            } else {
              xVel = -1;
              xPos = 2;
            }
          }
          //set the first bulelt
          std::list<Bullet*>::iterator it = freeBull.begin();
          (*it)->inUse(Vector2f(player->X()+xPos, player->Y()+yPos));
          (*it)->setVelo(xVel, 0);
          inUse.push_back(*it);
          it = freeBull.erase(it);
          //set the second bullet
          (*it)->inUse(Vector2f(player->X()+xPos, player->Y()+yPos+15));
          (*it)->setVelo(xVel, 0);
          inUse.push_back(*it);
          it = freeBull.erase(it);
        }
        if (keystate[SDLK_r]) {
          for (unsigned i = 0; i < sprites.size(); ++i) {
            sprites[i]->respawn();
          }
          player->respawn();
          for(std::list<Bullet*>::iterator it = inUse.begin(); it != inUse.end(); it++) {
            freeBull.push_back(*it);
            it = inUse.erase(it);
          }
          hud.setHealth(player->getHealth());
        }
      }
      if(event.type == SDL_KEYUP) {
        move = keystate[SDLK_w] | keystate[SDLK_a]<<1 | keystate[SDLK_s]<<2 | keystate[SDLK_d]<<3;
        player->updateMovement(move);
      } 
    }
    draw();
    update();
  }
}
