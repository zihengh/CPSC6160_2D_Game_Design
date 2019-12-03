#include "SDL.h"



class HealthBar {
public:
  HealthBar(const std::string&);
  void draw(SDL_Renderer* const renderer) const;
  void update(Uint32, int action);

  HealthBar(const HealthBar&) = delete;
  HealthBar& operator=(const HealthBar&) = delete;

  int getScaledWidth() const  { return barRect.w; }
  int getScaledHeight() const { return barRect.h; }
  bool Iswin(){return win;}
  void reset();
  void setSpeed(bool speed);
private:
  int startLocX;
  int startLocY;
  SDL_Rect barRect;
  int currentLength;
  int increment;  
  float interval; // elapsed time for each decrement (1000ms=1sec)
  int deltaTime;
  SDL_Color barColor;
  SDL_Color backColor;
  bool win;
  bool speedup;
};
