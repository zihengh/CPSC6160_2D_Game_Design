#ifndef CAMERA__H
#define CAMERA__H

#include <SDL.h>
//#include "renderContext.h"

class Camera  {
public:
  Camera();
  void reset();
  void updateCamera(); 
  void draw(SDL_Renderer* const renderer) const;
  int getCameraSpot(){return cameraSpot;}
private:
//  const RenderContext& rc;
//  SDL_Renderer * const renderer;
  bool rightForward;
  int cameraSpot;
  int viewWidth;
  int viewHeight;
};
#endif
