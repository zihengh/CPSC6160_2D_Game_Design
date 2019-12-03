#include "player.h"
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"

Player::Player( const std::string& name) :
  MultiSprite(name),
  observers(),
  collision(false),
  initialVelocity(getVelocity()),
  action(0),
  god(false)
{ }

Player::Player(const Player& s) :
  MultiSprite(s), 
  observers( s.observers ),
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  action(s.action),
  god(s.god)
  { }

Player& Player::operator=(const Player& s) {
  MultiSprite::operator=(s);
  collision = s.collision;
  observers = s.observers;
  initialVelocity = s.initialVelocity;
  return *this;
}

void Player::advanceFrame1(Uint32 ticks) {
	
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    if(currentFrame%2==0)
    {
      currentFrame = currentFrame+1;
    }
    else currentFrame = currentFrame-1;
		timeSinceLastFrame = 0;
    //std::cout<<currentFrame<<std::endl;
	}
  
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::updateplayer(Uint32 ticks) { 
  if ( !collision ) 
  {
    advanceFrame1(ticks);

    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
  }
  //stop();
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    int offset = rand()%5;
    offset = offset*(rand()%2?-1:1);
    const Vector2f off(offset, offset);
    (*ptr)->setPlayerPos( getPosition()+off );
    ++ptr;
  }
}

void Player::stop() { 
  if(action!=ENUM_ACTION_STOP)
  {
    action = ENUM_ACTION_STOP;
    currentFrame = 4;
    return;
  }
  
  
}

void Player::start() { 
  if(action!=ENUM_ACTION_ON)
  {
    action = ENUM_ACTION_ON;
    currentFrame = 6;
    return;
  }
  
  
}

/*void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} */
void Player::up()    { 
  if(action!=ENUM_ACTION_UP)
  {
    action = ENUM_ACTION_UP;
    currentFrame = 2;
  }
} 
void Player::down()  { 
  if(action!=ENUM_ACTION_DOWN)
  {
    action = ENUM_ACTION_DOWN;
    currentFrame = 0;
  }
}

int Player::getAction()
{
  return action;
}

int Player::getResult(int cameraSpot, int coachPos, int teacherPos)
{
  if(god)
  {
    return ENUM_END_CONTINUE;
  }
  if(teacherPos>getX() && teacherPos<getX()+getScaledWidth()*0.5)
  {
    if(action != ENUM_ACTION_STOP)
      return ENUM_END_TEACHER;
  }
  if(cameraSpot>getX() && cameraSpot<getX()+getScaledWidth())
  {
    if(action != ENUM_ACTION_STOP && action != ENUM_ACTION_UP)
      return ENUM_END_CAMERA;
  }
  if(coachPos>875 && coachPos<920)
  {
    if(action != ENUM_ACTION_STOP && action != ENUM_ACTION_DOWN)
      return ENUM_END_COACH;
  }
  return ENUM_END_CONTINUE;
}

