#ifndef ZIGEN_PLAYGROUND_PLAYGROUND_VIEW_H
#define ZIGEN_PLAYGROUND_PLAYGROUND_VIEW_H

#include <zigen-playground.h>

#include <memory>

namespace zigen_playground {

class PlaygroundView : public zukou::objects::IObject {
 public:
  PlaygroundView(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object);

  virtual bool Init() override final;
  virtual bool Draw() override final;
  virtual float Intersect(glm::vec3 origin, glm::vec3 direction) override final;

  virtual void RayEnter() override final;
  virtual void RayLeave() override final;
  virtual void RayMotion(
      glm::vec3 origin, glm::vec3 direction, uint32_t time) override final;
  virtual void RayButton(uint32_t serial, uint32_t time, uint32_t button,
      bool pressed) override final;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_PLAYGROUND_VIEW_H
