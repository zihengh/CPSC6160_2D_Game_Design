#include<sstream>
#include "healthBar.h"
#include "gameData.h"
#include "player.h"

HealthBar::HealthBar(const std::string& name) :  
  startLocX(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")),
  startLocY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  barRect( { Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/width"),
             Gamedata::getInstance().getXmlInt(name+"/height")}),
  currentLength(0), 
  increment(barRect.w/Gamedata::getInstance().getXmlInt(name+"/increment")),
  interval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  deltaTime(0),
  barColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/a")}
  ),
  backColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/a")}
  ),
  win(false),
  speedup(false)
{}

void HealthBar::update(Uint32 ticks, int action) {
  deltaTime += ticks;
  if ( deltaTime < interval ) {
    
    return;
  }
  deltaTime = 0;
  if(action==ENUM_ACTION_STOP)
  {
    if(currentLength<=0)
      return;
    currentLength -= 0.2*increment;
  }
  else 
  {
    if(currentLength>=barRect.w)
    {
      win=true;
      return;
    }
    if(speedup)
      currentLength += 0.6*increment;
    else currentLength += 0.2*increment;
  }
}

void HealthBar::draw(SDL_Renderer* const renderer) const {

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the health bar background color:
  SDL_SetRenderDrawColor( 
    renderer, backColor.r, backColor.g, backColor.b, backColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &barRect );

  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  int x = startLocX;
  int y = startLocY;
  SDL_Rect healthRect = {x, y, currentLength, barRect.h};
  SDL_RenderFillRect( renderer, &healthRect );

  // Set the  color for the health bar outline:
  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_RenderDrawRect( renderer, &barRect );
}

void HealthBar::reset()
{
  currentLength=0;
  win=false;
  speedup=false;
}

void HealthBar::setSpeed(bool speed)
{
  speedup=speed;
}