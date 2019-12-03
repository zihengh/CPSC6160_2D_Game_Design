// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"


//RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   //ImageFactory&  imageFactory = ImageFactory::getInstance();
   try {
      Engine engine;
      engine.play();
      //delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
