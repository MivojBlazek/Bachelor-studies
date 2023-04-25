/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>

void draw(GPUMemory &mem, DrawCommand cmd, uint32_t nofDraws){
  Program prg = mem.programs[cmd.programID];

  for (uint32_t i = 0; i < cmd.nofVertices; i++)
  {
    InVertex inVertex;
    OutVertex outVertex;
    inVertex.gl_DrawID = nofDraws;

    if (cmd.vao.indexBufferID < 0)
    {
      // neni zapnuty indexovani (test 4)
      inVertex.gl_VertexID = i;
    }
    else
    {
      // je zapnuty indexovani (test 5)
      int32_t bufferID = cmd.vao.indexBufferID;
      void const *indexBuffer = mem.buffers[bufferID].data;
      
      if (cmd.vao.indexType == IndexType::UINT8)
      {
        uint8_t *ind = (uint8_t *) ((uint64_t *)indexBuffer + cmd.vao.indexOffset);
        inVertex.gl_VertexID = ind[i];
      }
      else if (cmd.vao.indexType == IndexType::UINT16)
      {
        uint16_t *ind = (uint16_t *) ((uint64_t *)indexBuffer + cmd.vao.indexOffset);
        inVertex.gl_VertexID = ind[i];
      }
      else // (cmd.vao.indexType == IndexType::UINT32)
      {
        uint32_t *ind = (uint32_t *) ((uint64_t *)indexBuffer + cmd.vao.indexOffset);
        inVertex.gl_VertexID = ind[i];
      }
    }

    ShaderInterface si;
    si.uniforms = mem.uniforms;
    si.textures = mem.textures;

    //TODO (tests 7-9)
    if (cmd.vao.vertexAttrib[0].type != AttributeType::EMPTY)
    {
      //TODO data copy with size vertex attribut from buffer to offset with stride (krok)
      //TODO velikosti jsou v bytech
      //TODO atributy by mely byt cteny z adresy: buf_ptr + offset + stride * gl_VertexID
      uint64_t vaOffset = cmd.vao.vertexAttrib[0].offset;
      int32_t vaBuffer = cmd.vao.vertexAttrib[0].bufferID;
      uint64_t vaStride = cmd.vao.vertexAttrib[0].stride;
      AttributeType vaType = cmd.vao.vertexAttrib[0].type;
    }

    prg.vertexShader(outVertex, inVertex, si);
  }
}

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
  uint32_t nofDraws = 0;
  for (uint32_t i = 0; i < cb.nofCommands; i++)
  {
    if (cb.commands[i].type == CommandType::CLEAR)
    {
      clear(mem, cb.commands[i].data.clearCommand);
    }
    else if (cb.commands[i].type == CommandType::DRAW)
    {
      draw(mem, cb.commands[i].data.drawCommand, nofDraws++);
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

