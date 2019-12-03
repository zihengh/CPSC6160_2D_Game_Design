#ifndef HUD_H
#define HUD_H

#include "SDL.h"
#include "ioMod.h"


class hud {
public:
  hud();
  void draw(SDL_Renderer* const renderer, const IoMod *io) const;
  
private:

  SDL_Rect r;
    
};

#endif