#ifndef PLAYER__H
#define PLAYER__H

enum
{
  ENUM_ACTION_ON=0,
  ENUM_ACTION_UP,
  ENUM_ACTION_DOWN,
  ENUM_ACTION_STOP,
};

enum
{
  ENUM_END_TEACHER=0,
  ENUM_END_COACH,
  ENUM_END_CAMERA,
  ENUM_END_WIN,
  ENUM_END_CONTINUE,
  
};



#include "multisprite.h"
#include <cmath>
#include <string>
#include <list>
// In this example the player is derived from MultiSprite.
// However, there are many options.

class SmartSprite;

class Player : public MultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  void updateplayer(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );
  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  //void right();
  //void left();
  void up();
  void down();
  void stop();
  void start();
  int getAction();
  int getResult(int cameraSpot, int coachPos, int teacherPos);
  void turnGod(){god=!god;}
  bool isGod(){return god;}
protected:
  std::list<SmartSprite*> observers;
  void advanceFrame1(Uint32 ticks);
private:
  bool collision;
  Vector2f initialVelocity;
  int action;
  bool god;
};
#endif
