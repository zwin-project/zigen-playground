#ifndef ZIGEN_PLAYGROUND_CUBOID_VIEW_H
#define ZIGEN_PLAYGROUND_CUBOID_VIEW_H

#include <model.h>
#include <zigen-playground.h>

namespace zigen_playground {

class CuboidView : public zukou::objects::IObject {
 public:
  CuboidView(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::shared_ptr<model::Cuboid> cuboid);
  virtual bool Init() override final;
  virtual bool Draw() override final;
  virtual float Intersect(glm::vec3 origin, glm::vec3 direction) override final;

  virtual void RayEnter() override final;
  virtual void RayLeave() override final;
  virtual void RayMotion(
      glm::vec3 origin, glm::vec3 direction, uint32_t time) override final;
  virtual void RayButton(uint32_t serial, uint32_t time, uint32_t button,
      bool pressed) override final;

  virtual void DataDeviceEnter(uint32_t serial,
      std::weak_ptr<zukou::DataOffer> data_offer) override final;
  virtual void DataDeviceLeave() override final;
  virtual void DataDeviceMotion(
      uint32_t time, glm::vec3 origin, glm::vec3 direction) override final;
  virtual void DataDeviceDrop() override final;

  void TextDropped(int fd);

  void SetGeometry(glm::vec3 parent_position, glm::quat parent_quaternion);

 private:
  glm::mat4 GetTransformMatrix();

 private:
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::VirtualObject> virtual_object_;
  std::shared_ptr<model::Cuboid> cuboid_;
  glm::vec3 parent_position_;
  glm::quat parent_quaternion_;

  std::shared_ptr<zukou::OpenGLComponent> component_;
  std::shared_ptr<zukou::OpenGLShaderProgram> shader_;
  std::shared_ptr<zukou::OpenGLVertexBuffer> vertex_buffer_;
  std::shared_ptr<zukou::OpenGLElementArrayBuffer> element_array_buffer_;

  std::weak_ptr<zukou::DataOffer> data_offer_;
  uint32_t data_device_enter_serial_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_CUBOID_VIEW_H
