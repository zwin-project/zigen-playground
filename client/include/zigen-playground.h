#ifndef ZIGEN_PLAYGROUND_H
#define ZIGEN_PLAYGROUND_H

#include <zukou/zukou.h>

#include <glm/glm.hpp>
#include <string>

namespace zigen_playground {

class Playground : public zukou::objects::IObject {
 public:
  Playground(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object);

  void SetGeometry(glm::vec3 half_size, glm::vec3 position);

  virtual bool Init() final;
  virtual bool Draw() final;
  virtual float Intersect(glm::vec3 origin, glm::vec3 direction) final;
  virtual void RayEnter() final;
  virtual void RayLeave() final;
  virtual void RayMotion(
      glm::vec3 origin, glm::vec3 direction, uint32_t time) final;
  virtual void RayButton(
      uint32_t serial, uint32_t time, uint32_t button, bool pressed) final;

 private:
  glm::vec3 half_size_;
  glm::vec3 position_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_H
