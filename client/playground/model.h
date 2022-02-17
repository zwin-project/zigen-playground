#ifndef ZIGEN_PLAYGROUND_MODEL_H
#define ZIGEN_PLAYGROUND_MODEL_H

#include <glm/glm.hpp>
#include <memory>

namespace zigen_playground {
namespace model {

struct Resource {
  Resource(std::string type) : type(type) {}
  virtual ~Resource() {}

  std::string type;
};

struct Cuboid : public Resource {
  Cuboid(uint64_t id, glm::vec3 position, glm::vec3 half_size)
      : Resource("cuboid"), id(id), position(position), half_size(half_size){};

  uint64_t id;
  glm::vec3 position;
  glm::vec3 half_size;
};

struct Sphere : public Resource {
  Sphere(uint64_t id, glm::vec3 position, float r, uint32_t resolution,
      std::string texture_url)
      : Resource("sphere"),
        id(id),
        position(position),
        r(r),
        resolution(resolution),
        texture_url(texture_url) {}

  uint64_t id;
  glm::vec3 position;
  float r;
  uint32_t resolution;
  std::string texture_url;
};

struct Ray {
  Ray(uint64_t client_id, glm::vec3 origin, glm::vec3 target)
      : client_id(client_id), origin(origin), target(target) {}

  uint64_t client_id;
  glm::vec3 origin;
  glm::vec3 target;
};

}  // namespace model
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MODEL_H
