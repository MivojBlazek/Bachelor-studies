/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>
#include <stdio.h> //! test

struct Triangle{
  OutVertex points[3];
};


float max(float a, float b, float c){
  float max = a;
  if (b > max)
  {
    max = b;
  }
  if (c > max)
  {
    max = c;
  }
  return max;
}


float min(float a, float b, float c){
  float min = a;
  if (b < min)
  {
    min = b;
  }
  if (c < min)
  {
    min = c;
  }
  return min;
}

//asi zbytecna funkce
float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
  return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}


void rasterize(Frame &framebuffer, Triangle const &triangle, Program const &prg, DrawCommand cmd, ShaderInterface si){
  glm::vec3 point[3];
  glm::vec3 point2[3];
  for (uint32_t i = 0; i < 3; i++)
  {
    /* Perspektivni deleni */
    point[i].x = triangle.points[i].gl_Position.x / triangle.points[i].gl_Position.w;
    point[i].y = triangle.points[i].gl_Position.y / triangle.points[i].gl_Position.w;
    point[i].z = triangle.points[i].gl_Position.z / triangle.points[i].gl_Position.w;

    point2[i].x = triangle.points[i].gl_Position.x / triangle.points[i].gl_Position.w;
    point2[i].y = triangle.points[i].gl_Position.y / triangle.points[i].gl_Position.w;
    point2[i].z = triangle.points[i].gl_Position.z / triangle.points[i].gl_Position.w;
    
    /* View-port transformace */
    point[i].x = (point[i].x + 1) * (framebuffer.width / 2);
    point[i].y = (point[i].y + 1) * (framebuffer.height / 2);
    point[i].x = round(point[i].x); //? not sure
    point[i].y = round(point[i].y); //? not sure
    point[i].z = round(point[i].z); //? not sure
    //fprintf(stderr, "\n-------------\n%f, %f, %f\n", point[i].x, point[i].y, point[i].z); //! test
  }


  uint32_t min_x = (uint32_t) min(point[0].x, point[1].x, point[2].x);
  uint32_t max_x = (uint32_t) max(point[0].x, point[1].x, point[2].x);
  uint32_t min_y = (uint32_t) min(point[0].y, point[1].y, point[2].y);
  uint32_t max_y = (uint32_t) max(point[0].y, point[1].y, point[2].y);
  if ((max_x - min_x) > framebuffer.width)
  {
    min_x = 0;
    max_x = framebuffer.width - 1;
  }
  if ((max_y - min_y) > framebuffer.height)
  {
    min_y = 0;
    max_y = framebuffer.height - 1;
  }
  //fprintf(stderr, "\n----min max-----\n%d, %d, %d, %d\n", min_x, max_x, min_y, max_y); //! test

  int flag = 0;
  if (cmd.backfaceCulling)
  {
    //zahodit, kdyz je trojuhelnik clockwise
    float area = (point[1].x - point[0].x) * (point[2].y - point[0].y) - (point[2].x - point[0].x) * (point[1].y - point[0].y);
    if (area < 0)
    {
      flag = 1;
    }
  }

  for (uint32_t x = min_x; x <= max_x; x++)
  {
    for (uint32_t y = min_y; y <= max_y; y++)
    {
      float l0 = ((point[1].y - point[2].y) * (x - point[2].x) + (point[2].x - point[1].x) * (y - point[2].y)) / ((point[1].y - point[2].y) * (point[0].x - point[2].x) + (point[2].x - point[1].x) * (point[0].y - point[2].y));
      float l1 = ((point[2].y - point[0].y) * (x - point[2].x) + (point[0].x - point[2].x) * (y - point[2].y)) / ((point[1].y - point[2].y) * (point[0].x - point[2].x) + (point[2].x - point[1].x) * (point[0].y - point[2].y));
      float l2 = 1 - l0 - l1;
      
      if (l0 > 0 && l1 >= 0 && l2 >= 0 && !flag) //? >= >=
      {
        InFragment inFragment;
        OutFragment outFragment;

        inFragment.gl_FragCoord.x = (float)((float)x + 0.5);
        inFragment.gl_FragCoord.y = (float)((float)y + 0.5);

        inFragment.gl_FragCoord.z = (point2[0].z * l0 + point2[1].z * l1 + point2[2].z * l2);
      

        //for interpolate color of pixel we use center of it
        float l0 = ((point[1].y - point[2].y) * (inFragment.gl_FragCoord.x - point[2].x) + (point[2].x - point[1].x) * (inFragment.gl_FragCoord.y - point[2].y)) / ((point[1].y - point[2].y) * (point[0].x - point[2].x) + (point[2].x - point[1].x) * (point[0].y - point[2].y));
        float l1 = ((point[2].y - point[0].y) * (inFragment.gl_FragCoord.x - point[2].x) + (point[0].x - point[2].x) * (inFragment.gl_FragCoord.y - point[2].y)) / ((point[1].y - point[2].y) * (point[0].x - point[2].x) + (point[2].x - point[1].x) * (point[0].y - point[2].y));
        float l2 = 1 - l0 - l1;

        float h0 = triangle.points[0].gl_Position.w;
        float h1 = triangle.points[1].gl_Position.w;
        float h2 = triangle.points[2].gl_Position.w;

        float s = l0 / h0 + l1 / h1 + l2 / h2;
        //fprintf(stderr, "\n--- %f ----\n", s); //! test

        float new_l0 = l0 / (h0 * s);
        float new_l1 = l1 / (h1 * s);
        float new_l2 = l2 / (h2 * s);
        //fprintf(stderr, "\n-------\n%f, %f, %f\n", new_l0, new_l1, new_l2); //! test

        AttributeType frType = prg.vs2fs[0];
        if (frType != AttributeType::EMPTY)
        {
          if (frType == AttributeType::FLOAT)
          {
            //TODO
          }
          else if (frType == AttributeType::VEC2)
          {
            //TODO
          }
          else if (frType == AttributeType::VEC3)
          {
            glm::vec3 vAttrib1 = triangle.points[0].attributes[0].v3;
            glm::vec3 vAttrib2 = triangle.points[1].attributes[0].v3;
            glm::vec3 vAttrib3 = triangle.points[2].attributes[0].v3;
            inFragment.attributes[0].v3 = vAttrib1 * new_l0 + vAttrib2 * new_l1 + vAttrib3 * new_l2;
          }
          else if (frType == AttributeType::VEC4)
          {
            //TODO
          }
          else // (frType == AttributeType::UINT/UVEC2/UVEC3/UVEC4)
          {
            //TODO
          }
        }

        prg.fragmentShader(outFragment, inFragment, si);

        
        // PFO
        // depth test
        float depth = inFragment.gl_FragCoord.z;
        if (framebuffer.depth[(uint8_t)(inFragment.gl_FragCoord.x * inFragment.gl_FragCoord.y + inFragment.gl_FragCoord.x)] > depth) //? pixel incrementation incorrect
        {
          framebuffer.depth[(uint8_t)(inFragment.gl_FragCoord.x * inFragment.gl_FragCoord.y + inFragment.gl_FragCoord.x)] = depth;
          
          framebuffer.color[(uint8_t)((inFragment.gl_FragCoord.x * inFragment.gl_FragCoord.y + inFragment.gl_FragCoord.x))] = (uint8_t)(outFragment.gl_FragColor.x * 255.f);
          framebuffer.color[(uint8_t)((inFragment.gl_FragCoord.x * inFragment.gl_FragCoord.y + inFragment.gl_FragCoord.x) + 1)] = (uint8_t)(outFragment.gl_FragColor.y * 255.f);
          framebuffer.color[(uint8_t)((inFragment.gl_FragCoord.x * inFragment.gl_FragCoord.y + inFragment.gl_FragCoord.x) + 2)] = (uint8_t)(outFragment.gl_FragColor.z * 255.f);
        }
      }
    }
  }
}


void draw(GPUMemory &mem, DrawCommand cmd, uint32_t nofDraws){
  Program prg = mem.programs[cmd.programID];
  Triangle triangle;

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
          //TODO
        }
      }
    }
    prg.vertexShader(outVertex, inVertex, si);
    
    triangle.points[i % 3] = outVertex; //! problem u vice draw commandu imo (not sure jestli " % 3" to fixne)
    if (i % 3 == 2)
    {
      rasterize(mem.framebuffer, triangle, prg, cmd, si);
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

