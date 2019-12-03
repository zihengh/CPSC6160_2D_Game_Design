#include<sstream>
#include "Hud.h"
#include "gameData.h"
#include "clock.h"

hud::hud() :  
  r({Gamedata::getInstance().getXmlInt("Hud/loc/x"),
    Gamedata::getInstance().getXmlInt("Hud/loc/y"),
    Gamedata::getInstance().getXmlInt("Hud/width"),
    Gamedata::getInstance().getXmlInt("Hud/height")})
{}



void hud::draw(SDL_Renderer* const renderer, const IoMod *io) const {

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
    io->writeText("FPS: "+ fps.str(), r.x+10,r.y+10);
    int nLine(Gamedata::getInstance().getXmlInt("Hud/line"));
    for(int nl=0; nl<nLine; nl++)
    {
      std::stringstream opinfo;
      opinfo<<"Hud/l"<<nl+1;
      io->writeText(Gamedata::getInstance().getXmlStr(opinfo.str()), r.x+10,r.y+40+30*nl);
    }
}

