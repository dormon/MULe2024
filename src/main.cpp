#include<iostream>
#include<SDL.h>


int main(int argc,char*argv[]){
  std::cerr << "Hello World!" << std::endl;
  SDL_CreateWindow("MULe",0,0,1024,748,0);
  bool running = true;
  while(running){ // MAIN LOOP
    SDL_Event e;
    while(SDL_PollEvent(&e)){ // EVENT LOOP
      if(e.type == SDL_QUIT)running = false;
    }

  };

  return 0;
}
