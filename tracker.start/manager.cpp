#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "2waymultisprite.h"
#include "player.h"
#include "sprite.h"
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

bool Manager::checkForCollisions() const {
  std::vector<Drawable*>::const_iterator sprite = sprites.begin();
  while ( sprite != sprites.end() ) {
    if ( player->collidedWith(*sprite) ) return true;
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
  player->draw();
  io.printMessageValueAt("X: ", player->X(), 320, 80);
  io.printMessageValueAt("Y: ", player->Y(), 400, 80);
  if ( checkForCollisions() ) {
    io.printMessageAt("*** Oops ***, collision!", 320, 60);
  } else {
    io.printMessageAt("No Collision.", 320, 60);
  }
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
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
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