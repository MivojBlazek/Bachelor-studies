/*!
 * @file
 * @brief This file contains functions for model rendering
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#include <student/drawModel.hpp>
#include <student/gpu.hpp>
#include <stdio.h> //! test
///\endcond

void addDrawCommand(CommandBuffer &cb, Mesh const &mesh)
{
  if (mesh.diffuseTexture >= 0)
  {
    //TODO textures read_texture()...
  }

  cb.commands[cb.nofCommands].type = CommandType::DRAW;
  if (mesh.doubleSided)
  {
    cb.commands[cb.nofCommands].data.drawCommand.backfaceCulling = false;
  }
  else
  {
    cb.commands[cb.nofCommands].data.drawCommand.backfaceCulling = true;
  }
  cb.commands[cb.nofCommands].data.drawCommand.nofVertices = mesh.nofIndices;
  cb.commands[cb.nofCommands].data.drawCommand.programID = 0;

  cb.commands[cb.nofCommands].data.drawCommand.vao.indexBufferID = mesh.indexBufferID;
  cb.commands[cb.nofCommands].data.drawCommand.vao.indexOffset = mesh.indexOffset;
  cb.commands[cb.nofCommands].data.drawCommand.vao.indexType = mesh.indexType;

  cb.commands[cb.nofCommands].data.drawCommand.vao.vertexAttrib[0] = mesh.position;
  cb.commands[cb.nofCommands].data.drawCommand.vao.vertexAttrib[1] = mesh.normal;
  cb.commands[cb.nofCommands].data.drawCommand.vao.vertexAttrib[2] = mesh.texCoord;
  cb.nofCommands++;
}

glm::mat4 combineMatrix(glm::mat4 const &m1, glm::mat4 const &m2)
{
  return m1 * m2;
}

uint32_t prepareNode(GPUMemory &mem, CommandBuffer &cb, Node const &node, Model const &model, glm::mat4 prubeznaMatice, uint32_t drawID)
{
  if (node.mesh >= 0)
  {
    Mesh const &mesh = model.meshes[node.mesh];
    addDrawCommand(cb, mesh);
    drawID++;

    prubeznaMatice = combineMatrix(prubeznaMatice, node.modelMatrix);
    glm::mat4 inverseModelMatrix = glm::transpose(glm::inverse(prubeznaMatice));

    // test 36
    // uniforms
    mem.uniforms[10 + (drawID - 1) * 5].m4 = prubeznaMatice;
    mem.uniforms[10 + (drawID - 1) * 5 + 1].m4 = inverseModelMatrix;
    mem.uniforms[10 + (drawID - 1) * 5 + 2].v4 = mesh.diffuseColor;
    mem.uniforms[10 + (drawID - 1) * 5 + 3].i1 = mesh.diffuseTexture;
    mem.uniforms[10 + (drawID - 1) * 5 + 4].v1 = mesh.doubleSided;
    
  }

  // rekurzivni zkoumani potomku
  for(size_t i = 0; i < node.children.size(); i++)
  {
    drawID = prepareNode(mem, cb, node.children[i], model, prubeznaMatice, drawID);
  }
  return drawID;
}

/**
 * @brief This function prepares model into memory and creates command buffer
 *
 * @param mem gpu memory
 * @param commandBuffer command buffer
 * @param model model structure
 */
//! [drawModel]
void prepareModel(GPUMemory&mem,CommandBuffer&commandBuffer,Model const&model){
  (void)mem;
  (void)commandBuffer;
  (void)model;
  /// \todo Tato funkce připraví command buffer pro model a nastaví správně pamět grafické karty.<br>
  /// Vaším úkolem je správně projít model a vložit vykreslovací příkazy do commandBufferu.
  /// Zároveň musíte vložit do paměti textury, buffery a uniformní proměnné, které buffer command buffer využívat.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace a v testech.
  

  commandBuffer.commands[0].type = CommandType::CLEAR;
  commandBuffer.commands[0].data.clearCommand.clearColor = true;
  commandBuffer.commands[0].data.clearCommand.color = glm::vec4(0.1,0.15,0.1,1);
  commandBuffer.commands[0].data.clearCommand.clearDepth = true;
  commandBuffer.commands[0].data.clearCommand.depth = 10e10f;
  commandBuffer.nofCommands = 1;
  
  glm::mat4 matrix = glm::mat4(1.f); // jednotkova matice
  uint32_t drawID = 0;

  size_t nofRoots = model.roots.size();
  for (size_t i = 0; i < nofRoots; i++)
  {
    drawID = prepareNode(mem, commandBuffer, model.roots[i], model, matrix, drawID);
  }

  // test 32
  mem.programs[0].vertexShader = drawModel_vertexShader;
  mem.programs[0].fragmentShader = drawModel_fragmentShader;

  // test 33 //? not sure, imo to musi byt nejak precteny, ale to nejde
  mem.programs[0].vs2fs[0] = AttributeType::VEC3; //position
  mem.programs[0].vs2fs[1] = AttributeType::VEC3; //normal
  mem.programs[0].vs2fs[2] = AttributeType::VEC2; //texCoord
  mem.programs[0].vs2fs[3] = AttributeType::UINT; //drawID

  // test 34
  for (uint32_t i = 0; i < model.buffers.size(); i++)
  {
    mem.buffers[i].data = model.buffers[i].data;
    mem.buffers[i].size = model.buffers[i].size;
  }
  
  // test 35
  for (uint32_t i = 0; i < model.textures.size(); i++)
  {
    mem.textures[i].channels = model.textures[i].channels;
    mem.textures[i].data = model.textures[i].data;
    mem.textures[i].height = model.textures[i].height;
    mem.textures[i].width = model.textures[i].width;
  }

}
//! [drawModel]

/**
 * @brief This function represents vertex shader of texture rendering method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param si shader interface
 */
//! [drawModel_vs]
void drawModel_vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  (void)outVertex;
  (void)inVertex;
  (void)si;
  /// \todo Tato funkce reprezentujte vertex shader.<br>
  /// Vaším úkolem je správně trasnformovat vrcholy modelu.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
}
//! [drawModel_vs]

/**
 * @brief This functionrepresents fragment shader of texture rendering method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param si shader interface
 */
//! [drawModel_fs]
void drawModel_fragmentShader(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&si){
  (void)outFragment;
  (void)inFragment;
  (void)si;
  /// \todo Tato funkce reprezentujte fragment shader.<br>
  /// Vaším úkolem je správně obarvit fragmenty a osvětlit je pomocí lambertova osvětlovacího modelu.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
}
//! [drawModel_fs]

