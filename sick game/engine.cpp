#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "player.h"
#include "smartSprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "sprite.h"
//#include "twMultiSprite.h"


Engine::~Engine() { 
//  delete star;
//  delete spinningStar;
  std::cout << "Terminating program" << std::endl;
  std::vector<SmartSprite*> :: const_iterator it = vSprites.begin();
  while(it!=vSprites.end())
  { 
    delete *it;
    it++;
  }
  std::vector<Sprite*> :: const_iterator ie = endSprites.begin();
  while(ie!=endSprites.end())
  { 
    delete *ie;
    ie++;
  }
  delete teacher;
  delete coach;
  delete player;
  delete healthBar;
  //DestroyHud();
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  sound(),
  renderer( rc.getRenderer() ),
  topDoor("door", Gamedata::getInstance().getXmlInt("door/factor") ),
  floor("floor", Gamedata::getInstance().getXmlInt("floor/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("boy")),
  teacher(new MultiSprite("teacher")),
  coach(new MultiSprite("coach")),
  healthBar(new HealthBar("HealthBar")),
  camera(),
  Hud(),
  vSprites(0),
  endSprites(0),
  strategies(),
  currentStrategy(0),
  currentSprite(0),
  cameraSpot(50),
  presenthud(false),
  makeVideo( false ),
  status(ENUM_END_CONTINUE)
{
  vSprites.reserve(8);
  for(int nboy = 0; nboy < 4; nboy++)
  {
    SmartSprite* boy = new SmartSprite("normal", player->getPosition(), player->getScaledWidth(), player->getScaledHeight());
    boy->setPosition(Vector2f(20+nboy*180,140));
    vSprites.emplace_back(boy);
    player->attach( vSprites[nboy] );
    boy->setScale(0.3);
  }
  for(int nEnd = ENUM_END_TEACHER; nEnd < ENUM_END_CONTINUE; nEnd++)
  {
    Sprite* end = new Sprite(end_names[nEnd]);
    
    endSprites.emplace_back(end);
    end->setPosition(Vector2f(Gamedata::getInstance().getXmlInt("view/width")*0.5-end->getScaledWidth()*0.5, 
      Gamedata::getInstance().getXmlInt("view/height")*0.5-end->getScaledHeight()*0.5));
    
  }
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
  player->setScale(0.3);
  player->stop();
  teacher->setScale(1.5);
  coach->setScale(0.6);
  Viewport::getInstance().setObjectToTrack(player);
  //twMultiSprite* boy = new twMultiSprite("boy");
  
  //vSprites.emplace_back(camera);
  //vSprites.emplace_back(teacher);
  //vSprites.emplace_back(boy);
  std::cout << "Loading complete" << std::endl;
}

void Engine::reset()
{
  status = ENUM_END_CONTINUE;
  healthBar->reset();
  player->stop();
  camera.reset();
  static_cast<MultiSprite*>(teacher)->reset();
  static_cast<MultiSprite*>(coach)->reset();
}

void Engine::draw() const {
  floor.draw();
  coach->draw();
  topDoor.draw();

  std::vector<SmartSprite*> :: const_iterator it = vSprites.begin();
  while(it!=vSprites.end())
  {
    (*it)->draw();
    it++;
  }
  //camera->draw();
  
  player->draw();
  teacher->draw();
  viewport.draw();
  if(presenthud)
  {
    //CreateHud();
    Hud.draw(renderer, &io);
  }
  camera.draw(renderer);
  healthBar->draw(renderer);
  if(status<ENUM_END_CONTINUE)
  {
    endSprites[status]->draw();
  }
  if(player->isGod())
  {
    io.writeText("God mode on", 30,420);
  }
  SDL_RenderPresent(renderer);
  
}

void Engine::checkForCollisions() {
  auto it = vSprites.begin();
  while ( it != vSprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = vSprites.erase(it);
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  if(healthBar->Iswin())
  {
    //clock.pause();
    status = ENUM_END_WIN;
    return;
  }
  if(status == ENUM_END_CONTINUE)
    status = player->getResult(camera.getCameraSpot(), coach->getX(), teacher->getX());

  if(status != ENUM_END_CONTINUE)
  {
    if(status==ENUM_END_TEACHER)
      static_cast<MultiSprite*>(teacher)->freeze();
    if(status==ENUM_END_COACH)
      static_cast<MultiSprite*>(coach)->freeze();
    //clock.pause();
    return;
  }
  //checkForCollisions();
  std::vector<SmartSprite*> :: const_iterator it = vSprites.begin();
  while(it!=vSprites.end())
  {
    (*it)->update(ticks);
    it++;
  }
  //camera->update(ticks);
  teacher->update(ticks);
  coach->update(ticks);
  player->updateplayer(ticks);
  healthBar->update(ticks, player->getAction());
  floor.update(); // always update viewport last
  topDoor.update();
  viewport.update(); // always update viewport last
  camera.updateCamera();
}

void Engine::switchSprite(){
/*  ++currentSprite;

  currentSprite = currentSprite % (int)vSprites.size();
  Viewport::getInstance().setObjectToTrack(vSprites[currentSprite]);*/
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if (keystate[SDL_SCANCODE_F1]) 
        {
          presenthud = !presenthud;
        }
        if (keystate[SDL_SCANCODE_R]) {
        if(status!=ENUM_END_CONTINUE)
          reset();
        }
        if (keystate[SDL_SCANCODE_G]) {
          player->turnGod();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      /*if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }*/
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->up();
        healthBar->setSpeed(false);
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->down();
        healthBar->setSpeed(false);
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->stop();
        healthBar->setSpeed(false);
      }
      if (keystate[SDL_SCANCODE_SPACE]) {
        static_cast<Player*>(player)->start();
        healthBar->setSpeed(true);
      }
      
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}

void Engine::CreateHud() const
{
    
    //presenthud = !presenthud;
    // Setup renderer
    //SDL_Window* window = rc.getWindow();
    //hud =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the color for the background:
    SDL_SetRenderDrawColor( renderer, 208, 209, 210, 255 );

    // Clear winow
    //SDL_RenderClear( hud );

    SDL_Rect r;
    r.x = Gamedata::getInstance().getXmlInt("Hud/loc/x");
    r.y = Gamedata::getInstance().getXmlInt("Hud/loc/y");
    r.w = Gamedata::getInstance().getXmlInt("Hud/width");
    r.h = Gamedata::getInstance().getXmlInt("Hud/height");

    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

    // Render rect
    SDL_RenderFillRect( renderer, &r );

    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
    SDL_RenderDrawRect( renderer, &r );
    std::stringstream fps;
    fps << Clock::getInstance().getFps();
    io.writeText("FPS: "+ fps.str(), r.x+10,r.y+10);
    int nLine(Gamedata::getInstance().getXmlInt("Hud/line"));
    for(int nl=0; nl<nLine; nl++)
    {
      std::stringstream opinfo;
      opinfo<<"Hud/l"<<nl+1;
      io.writeText(Gamedata::getInstance().getXmlStr(opinfo.str()), r.x+10,r.y+40+30*nl);
    }
}


