/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>


struct Triangle{
  OutVertex points[3];
};


void loadVertex(InVertex inVertex, VertexArray const &vao, uint32_t tId_i){
  //TODO
}


void loadTriangle(Triangle &triangle, Program const &prg, VertexArray const &vao, uint32_t tId, ShaderInterface si){
  for(uint32_t i = 0; i < tId; i++) // smyčka přes vrcholy trojúhelníku
  {
    InVertex inVertex;
    loadVertex(inVertex, vao, tId+i);
    prg.vertexShader(triangle.points[i], inVertex, si);
  }
}


void draw(GPUMemory &mem, DrawCommand cmd, uint32_t nofDraws){
  Program prg = mem.programs[cmd.programID];

  for (uint32_t i = 0; i < cmd.nofVertices; i++)
  {
    InVertex inVertex;
    OutVertex outVertex;
    inVertex.gl_DrawID = nofDraws;

    /* Computing vertexID */
    if (cmd.vao.indexBufferID < 0)
    {
      // neni zapnuty indexovani
      inVertex.gl_VertexID = i;
    }
    else
    {
      // je zapnuty indexovani
      int32_t bufferID = cmd.vao.indexBufferID;
      void const *indexBuffer = mem.buffers[bufferID].data;
      
      if (cmd.vao.indexType == IndexType::UINT8)
      {
        uint8_t *ind = (uint8_t *) ((uint8_t *)indexBuffer + cmd.vao.indexOffset);
        inVertex.gl_VertexID = ind[i];
      }
      else if (cmd.vao.indexType == IndexType::UINT16)
      {
        uint16_t *ind = (uint16_t *) ((uint8_t *)indexBuffer + cmd.vao.indexOffset);
        inVertex.gl_VertexID = ind[i];
      }
      else // (cmd.vao.indexType == IndexType::UINT32)
      {
        uint32_t *ind = (uint32_t *) ((uint8_t *)indexBuffer + cmd.vao.indexOffset);
        inVertex.gl_VertexID = ind[i];
      }
    }

    ShaderInterface si;
    si.uniforms = mem.uniforms;
    si.textures = mem.textures;


    /* Reading vertex attributes */
    for (uint8_t j = 0; j < maxAttributes; j++)
    {
      AttributeType vaType = cmd.vao.vertexAttrib[j].type;
      if (vaType != AttributeType::EMPTY)
      {
        int32_t vaBuffer = cmd.vao.vertexAttrib[j].bufferID;
        uint64_t vaOffset = cmd.vao.vertexAttrib[j].offset;
        uint64_t vaStride = cmd.vao.vertexAttrib[j].stride;
        void const *vertexBuffer = mem.buffers[vaBuffer].data;

        if (vaType == AttributeType::FLOAT)
        {
          float *vAttrib = (float *) ((uint8_t *)vertexBuffer + vaOffset + vaStride * inVertex.gl_VertexID);
          inVertex.attributes[j].v1 = *vAttrib;
        }
        else if (vaType == AttributeType::VEC2)
        {
          glm::vec2 *vAttrib = (glm::vec2 *) ((uint8_t *)vertexBuffer + vaOffset + vaStride * inVertex.gl_VertexID);
          inVertex.attributes[j].v2 = *vAttrib;
        }
        else if (vaType == AttributeType::VEC3)
        {
          glm::vec3 *vAttrib = (glm::vec3 *) ((uint8_t *)vertexBuffer + vaOffset + vaStride * inVertex.gl_VertexID);
          inVertex.attributes[j].v3 = *vAttrib;
        }
        else if (vaType == AttributeType::VEC4)
        {
          glm::vec4 *vAttrib = (glm::vec4 *) ((uint8_t *)vertexBuffer + vaOffset + vaStride * inVertex.gl_VertexID);
          inVertex.attributes[j].v4 = *vAttrib;
        }
        else // (vaType == AttributeType::UINT/UVEC2/UVEC3/UVEC4)
        {
          //TODO dodelat asi
        }
      }
    }
    prg.vertexShader(outVertex, inVertex, si);

    if (cmd.backfaceCulling)
    {
      
    }
    else
    {
      
    }
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

