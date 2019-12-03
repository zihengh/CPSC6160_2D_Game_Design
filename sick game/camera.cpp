#include "gameData.h"
#include "camera.h"

Camera::Camera() :
//  rc( RenderContext::getInstance() ),
//  renderer( rc.getRenderer() ),
  rightForward(false),
  cameraSpot(1000),
  viewWidth(Gamedata::getInstance().getXmlInt("view/width")),
  viewHeight(Gamedata::getInstance().getXmlInt("view/height"))
{

}

void Camera::updateCamera() 
{
    if(rightForward)
    {
        cameraSpot++;
        if(cameraSpot>viewWidth)
            rightForward=false;
        return;
    }
    cameraSpot--;
    if(cameraSpot<0)
        rightForward=true;
  
}

void Camera::draw(SDL_Renderer* const renderer) const
{
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );


    int x1=480,  Interval=50, h=viewHeight-155;
    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 0, 255, 255, 100 );
    for(int nRow=155; nRow<viewHeight; nRow++)
    {
      for(int nCol=0; nCol<viewWidth; nCol++)
      {
        /*if(viewHeight*nCol<(viewHeight-nRow)*(cameraSpot-x1-Interval)+viewHeight*x1)
          continue;
        if(viewHeight*nCol>(viewHeight-nRow)*(cameraSpot-x1+Interval)+viewHeight*x1)
          continue;*/
        if(h*nCol<(viewHeight-nRow)*(cameraSpot-x1-Interval)+h*x1)
          continue;
        if(h*nCol>(viewHeight-nRow)*(cameraSpot-x1+Interval)+h*x1)
          continue;
        SDL_RenderDrawPoint(renderer, nCol, nRow);
      }
    }
    
    //SDL_RenderDrawRect( renderer, &r );
    
}

void Camera::reset()
{
  rightForward=false;
  cameraSpot=1000;
}