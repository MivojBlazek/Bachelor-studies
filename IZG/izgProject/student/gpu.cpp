/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>

void clear(GPUMemory &mem, ClearCommand cmd){
  if(cmd.clearColor)
  {
    float red = cmd.color.r;
    float green = cmd.color.g;
    float blue = cmd.color.b;
    float alpha = cmd.color.a;
    
    for (uint32_t x = 0; x < mem.framebuffer.width * mem.framebuffer.height * 4; x += 4)
    {
      mem.framebuffer.color[x] = (uint8_t) (red * 255.f);
      mem.framebuffer.color[x + 1] = (uint8_t) (green * 255.f);
      mem.framebuffer.color[x + 2] = (uint8_t) (blue * 255.f);
      mem.framebuffer.color[x + 3] = (uint8_t) (alpha * 255.f);
    }
  }
  if(cmd.clearDepth)
  {
    float depth = cmd.depth;

    for (uint32_t x = 0; x < mem.framebuffer.width * mem.framebuffer.height; x++)
    {
      mem.framebuffer.depth[x] = depth;
    }
  }
}

//! [gpu_execute]
void gpu_execute(GPUMemory&mem,CommandBuffer &cb){
  (void)mem;
  (void)cb;
  /// \todo Tato funkce reprezentuje funkcionalitu grafické karty.<br>
  /// Měla by umět zpracovat command buffer, čistit framebuffer a kresli.<br>
  /// mem obsahuje paměť grafické karty.
  /// cb obsahuje command buffer pro zpracování.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
  for (uint32_t i = 0; i < cb.nofCommands; i++)
  {
    if (cb.commands[i].type == CommandType::CLEAR)
    {
      clear(mem, cb.commands[i].data.clearCommand);
    }
    else if (cb.commands[i].type == CommandType::DRAW)
    {
      //TODO kresleni mozna
    }
  }
}
//! [gpu_execute]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.data)return glm::vec4(0.f);
  auto uv1 = glm::fract(uv);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  //auto t   = glm::fract(uv2);
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  for(uint32_t c=0;c<texture.channels;++c)
    color[c] = texture.data[(pix.y*texture.width+pix.x)*texture.channels+c]/255.f;
  return color;
}

