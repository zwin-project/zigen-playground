#ifndef ZIGEN_PLAYGROUND_PEDESTAL_H
#define ZIGEN_PLAYGROUND_PEDESTAL_H

#include <zukou/zukou.h>

#include <memory>

class Pedestal : public zukou::objects::IObject {
 public:
  static std::shared_ptr<Pedestal> Create(
      std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::CuboidWindow> cuboid_window);

  virtual bool Init() final;
  virtual bool Draw() final;
  virtual float Intersect(glm::vec3 origin, glm::vec3 direction) final;
  virtual void RayEnter() final;
  virtual void RayLeave() final;
  virtual void RayMotion(
      glm::vec3 origin, glm::vec3 direction, uint32_t time) final;
  virtual void RayButton(
      uint32_t serial, uint32_t time, uint32_t button, bool pressed) final;
  virtual void RayAxis(
      uint32_t time, uint32_t axis, float value) override final;
  virtual void RayFrame() override final;
  virtual void RayAxisDiscrete(uint32_t axis, int32_t discrete) override final;

  virtual void DataDeviceEnter(uint32_t serial,
      std::weak_ptr<zukou::DataOffer> data_offer) override final;
  virtual void DataDeviceLeave() override final;
  virtual void DataDeviceMotion(
      uint32_t time, glm::vec3 origin, glm::vec3 direction) override final;
  virtual void DataDeviceDrop() override final;

  void SetGeometry(float r, glm::vec3 position, glm::quat quaternion);

 private:
  Pedestal(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::CuboidWindow> cuboid_window);

  std::shared_ptr<zukou::OpenGLElementArrayBuffer> CreateElementArrayBuffer(
      uint32_t resolution, uint32_t threshold, uint32_t *count);

  std::shared_ptr<zukou::OpenGLVertexBuffer> CreateVertexBuffer(
      uint32_t resolution, uint32_t threshold);

 private:
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::CuboidWindow> cuboid_window_;

  bool needs_redraw_;
  float radius_;
  glm::quat quaternion_;
  uint32_t resolution_;
  uint32_t threshold_;
  glm::vec3 position_;

  std::shared_ptr<zukou::OpenGLComponent> component_;
  std::shared_ptr<zukou::OpenGLShaderProgram> shader_;
  std::shared_ptr<zukou::OpenGLVertexBuffer> vertex_buffer_;
  std::shared_ptr<zukou::OpenGLElementArrayBuffer> element_array_buffer_;
};

#endif  //  ZIGEN_PLAYGROUND_PEDESTAL_H
