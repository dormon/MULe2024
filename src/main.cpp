#include<iostream>
#include<SDL.h>
#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>
#include<videoReader.h>

using namespace ge::gl;

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "."
#endif

int main(int argc,char*argv[]){
  std::cerr << "Hello World!" << std::endl;
  auto window  = SDL_CreateWindow("MULe",0,0,1024,748,SDL_WINDOW_OPENGL);
  auto context = SDL_GL_CreateContext(window);

  ge::gl::init();

  auto vs = std::make_shared<Shader>(GL_VERTEX_SHADER,R".(
  #version 450

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
    gl_Position = vec4(vTexCoord*2-1,0,1);
    vTexCoord.y = 1-vTexCoord.y;

  }

  ).");

  auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER,R".(
  #version 450
  
  in vec2 vTexCoord;

  layout(binding=0)uniform sampler2D tex;

  out vec4 fColor;

  uniform int enableFilter = 0;

  void main(){
    vec4 color = texture(tex,vTexCoord);
    if(enableFilter == 0){
      fColor = color;
      return;
    }
    //any filter
    //
    color = vec4(color.r>.5);
    int kernel[] = int[](
        1, 9, 3,11,
       13, 5,15, 7,
        4,12, 2,10,
       16, 8,14, 6
    );
    
    vec2 pixCoord = vTexCoord;
    
    ivec2 kernelCoord = ivec2(fract(vTexCoord.x*100.f)*4.f,fract(vTexCoord.y*100.f)*4.f);
    
    float threshold = float(kernel[kernelCoord.y*4+kernelCoord.x]) / 16.f;
    
    vec3  ditheredColor = vec3(greaterThan(color.rgb,vec3(threshold)));



    fColor = vec4(ditheredColor,1);
  }

  ).");

  auto prg = std::make_shared<Program>(vs,fs);

  
  auto video = Video(CMAKE_ROOT_DIR "/res/mtm.mp4");

  auto texture = std::make_shared<Texture>(GL_TEXTURE_2D,GL_RGB8,1,video.getWidth(),video.getHeight());
  texture->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  texture->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  texture->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP);
  texture->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP);

  
    
  bool running = true;
  while(running){ // MAIN LOOP
    SDL_Event e;
    while(SDL_PollEvent(&e)){ // EVENT LOOP
      if(e.type == SDL_QUIT)running = false;
      if(e.type == SDL_KEYDOWN){
        if(e.key.keysym.sym == SDLK_f)
          prg->set1i("enableFilter",1);
        if(e.key.keysym.sym == SDLK_g)
          prg->set1i("enableFilter",0);
      }

    }

    glClearColor(.2,.2,.2,1);
    glClear(GL_COLOR_BUFFER_BIT);

    texture->setData2D(video.getData(),GL_BGR,GL_UNSIGNED_BYTE);

    texture->bind(0);//bind to texture unit 0
    prg->use();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    SDL_GL_SwapWindow(window);
  };

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);


  return 0;
}
