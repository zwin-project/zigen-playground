#ifndef ZIGEN_PLAYGROUND_SPHERE_VIEW_H
#define ZIGEN_PLAYGROUND_SPHERE_VIEW_H

#include <zukou/zukou.h>

#include "model.h"

namespace zigen_playground {

class SphereView : public zukou::objects::IObject {
 public:
  SphereView(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::shared_ptr<model::Sphere> sphere);

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

  glm::mat4 GetTransformMatrix();

 private:
  std::shared_ptr<zukou::OpenGLVertexBuffer> CreateVertexBuffer(
      uint32_t resolution);

  std::shared_ptr<zukou::OpenGLElementArrayBuffer>
  CreateFrameElementArrayBuffer(uint32_t resolution, uint32_t* count);

  std::shared_ptr<zukou::OpenGLElementArrayBuffer>
  CreateTextureElementArrayBuffer(uint32_t resolution, uint32_t* count);

  void OnTextureLoaded(uint32_t width, uint32_t height, uint32_t pixel_size,
      std::function<void(void* data)> writer);

 private:
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::VirtualObject> virtual_object_;
  std::shared_ptr<model::Sphere> sphere_;
  glm::vec3 parent_position_;
  glm::quat parent_quaternion_;

  std::shared_ptr<zukou::OpenGLComponent> component_;
  std::shared_ptr<zukou::OpenGLShaderProgram> shader_;
  std::shared_ptr<zukou::OpenGLVertexBuffer> vertex_buffer_;
  std::shared_ptr<zukou::OpenGLElementArrayBuffer> element_array_buffer_;
  std::shared_ptr<zukou::OpenGLTexture> texture_;

  std::weak_ptr<zukou::DataOffer> data_offer_;
  uint32_t data_device_enter_serial_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_SPHERE_VIEW_H
