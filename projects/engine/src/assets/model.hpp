#ifndef MANTA_MODEL_H
#define MANTA_MODEL_H

#include <vector>
#include <map>
#include <string>

#include <glm/glm/glm.hpp>

class Shader;
class Model;
class Entity;
class Renderer;
class Resources;

class VertexBuffer {
   public:
      virtual void Populate(Model* model) = 0;
      virtual void Draw(Renderer* renderer, Entity* entity, Shader* shader) = 0; // Model::Draw wraps around this

      virtual ~VertexBuffer() = 0;
};

class Model {
   public:
      struct Vertex
      {
	 glm::vec3 position;
   	 glm::vec3 normal;
   	 glm::vec2 uv0;

   	 bool operator== (const Vertex& other) const { return position == other.position && normal == other.normal && uv0 == other.uv0; }
      };

      std::vector<Vertex> vertices;
      std::vector<unsigned int> triangles;

      std::string name = "Unnamed model"; // If the format supports it, we extract the name

      VertexBuffer* vertexBuffer = nullptr;
      Shader* shader = nullptr;

      void Draw(Renderer* renderer, Resources* resources, Entity* entity);
};

class ModelLoader {
   public:
      Model* LoadModel(std::string path);
      std::map<std::string, Model*> loadedModels;
};

#endif
