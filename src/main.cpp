#include<iostream>
#include<SDL.h>
#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>

using namespace ge::gl;


int main(int argc,char*argv[]){
  std::cerr << "Hello World!" << std::endl;
  auto window  = SDL_CreateWindow("MULe",0,0,1024,748,SDL_WINDOW_OPENGL);
  auto context = SDL_GL_CreateContext(window);

  ge::gl::init();

  auto vs = std::make_shared<Shader>(GL_VERTEX_SHADER,R".(
  #version 330

  void main(){
    if(gl_VertexID == 0)gl_Position = vec4(0,0,0,1);
    if(gl_VertexID == 1)gl_Position = vec4(1,0,0,1);
    if(gl_VertexID == 2)gl_Position = vec4(0,1,0,1);
    if(gl_VertexID == 3)gl_Position = vec4(1,1,0,1);

  }

  ).");

  auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER,R".(
  #version 330

  out vec4 fColor;
  void main(){
    fColor = vec4(1);
  }

  ).");

  auto prg = std::make_shared<Program>(vs,fs);

  bool running = true;
  while(running){ // MAIN LOOP
    SDL_Event e;
    while(SDL_PollEvent(&e)){ // EVENT LOOP
      if(e.type == SDL_QUIT)running = false;
    }

    glClearColor(1,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    prg->use();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    SDL_GL_SwapWindow(window);
  };

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);


  return 0;
}
