#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "camera.h"
#include "healthBar.h"
#include "sound.h"
#include "Hud.h"

class Player;
class SmartSprite;
class CollisionStrategy;
class Sprite;

const std::string end_names[] = {"endteacher", "endcoach", "endcamera", "endwin"};

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;
  SDLSound sound;
  SDL_Renderer * const renderer;
  World topDoor;
  World floor;
  Viewport& viewport;

 // Drawable* star;
 // Drawable* spinningStar;
  Player* player;
  Drawable* teacher;
  Drawable* coach;
  HealthBar* healthBar;
  Camera camera;
  hud Hud;
  //Drawable* camera;
  std::vector<SmartSprite*> vSprites;
  std::vector<Sprite*> endSprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  int currentSprite;
  int cameraSpot;
  //SDL_Renderer*  hud;
  bool presenthud;
  bool makeVideo;
  int status;
  void draw() const;
  void update(Uint32);
  void reset();
  void printScales() const;
  void checkForCollisions();
  void CreateHud() const;
  
  //void DestroyHud();
};

#endif