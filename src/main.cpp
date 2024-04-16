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

  out vec2 vTexCoord;

  void main(){
    // 0&1 -> 0
    // 1&1 -> 1
    // 2&1 -> 0
    // 3&1 -> 1
    // 0>>1 -> 0
    // 1>>1 -> 0
    // 2>>1 -> 1
    // 3>>1 -> 1
    vTexCoord = vec2(gl_VertexID&1,gl_VertexID>>1);
    gl_Position = vec4(vTexCoord,0,1);

  }

  ).");

  auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER,R".(
  #version 330
  
  in vec2 vTexCoord;

  out vec4 fColor;
  void main(){
    fColor = vec4(vTexCoord,0,1);
  }

  ).");

  auto prg = std::make_shared<Program>(vs,fs);

  auto texture = std::make_shared<Texture>(GL_TEXTURE_2D,GL_RGB8I,1,10,10);

  bool running = true;
  while(running){ // MAIN LOOP
    SDL_Event e;
    while(SDL_PollEvent(&e)){ // EVENT LOOP
      if(e.type == SDL_QUIT)running = false;
    }

    glClearColor(.2,.2,.2,1);
    glClear(GL_COLOR_BUFFER_BIT);

    prg->use();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    SDL_GL_SwapWindow(window);
  };

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);


  return 0;
}
