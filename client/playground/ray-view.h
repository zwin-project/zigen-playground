#ifndef ZIGEN_PLAYGROUND_RAY_VIEW_H
#define ZIGEN_PLAYGROUND_RAY_VIEW_H

#include <zukou/zukou.h>

#include "model.h"

namespace zigen_playground {

class RayView : public zukou::primitives::IPrimitive {
 public:
  RayView(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::shared_ptr<model::Ray> ray);

  virtual bool Init() override final;
  virtual bool Draw() override final;

  void SetGeometry(glm::vec3 position, glm::quat quaternion);

  void UpdateRay(std::shared_ptr<model::Ray> ray);

  uint64_t GetId();

 private:
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::VirtualObject> virtual_object_;
  std::shared_ptr<model::Ray> ray_;

  glm::vec3 position_;
  glm::quat quaternion_;

  bool needs_redraw_;

  std::shared_ptr<zukou::OpenGLComponent> component_;
  std::shared_ptr<zukou::OpenGLShaderProgram> shader_;
  std::shared_ptr<zukou::OpenGLVertexBuffer> vertex_buffer_;
  std::shared_ptr<zukou::OpenGLElementArrayBuffer> element_array_buffer_;
  std::shared_ptr<zukou::OpenGLTexture> texture_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_RAY_VIEW_H
