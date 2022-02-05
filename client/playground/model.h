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
  Cuboid(glm::vec3 position, glm::vec3 half_size)
      : Resource("cuboid"), position(position), half_size(half_size){};

  glm::vec3 position;
  glm::vec3 half_size;
};

}  // namespace model
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MODEL_H
