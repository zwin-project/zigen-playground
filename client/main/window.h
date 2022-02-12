#ifndef ZIGEN_PLAYGROUND_MAIN_WINDOW_H
#define ZIGEN_PLAYGROUND_MAIN_WINDOW_H

#include <zigen-playground.h>
#include <zukou/zukou.h>

#include "pedestal.h"

class Window : public zukou::CuboidWindow {
 public:
  static std::shared_ptr<Window> Create(std::shared_ptr<zukou::Application> app,
      float radius, std::string host, std::string port, std::string user_id);

  bool Init();

 private:
  virtual bool Frame() override final;

  virtual void RayLeave([[maybe_unused]] uint32_t serial) override final;

  virtual void RayMotion(
      uint32_t time, glm::vec3 origin, glm::vec3 direction) override final;

  virtual void RayButton(uint32_t serial, uint32_t time, uint32_t button,
      bool pressed) override final;

  virtual void RayAxis(
      uint32_t time, uint32_t axis, float value) override final;

  virtual void RayFrame() override final;

  virtual void RayAxisDiscrete(uint32_t axis, int32_t discrete) override final;

  virtual void DataDeviceEnter(uint32_t serial, glm::vec3 origin,
      glm::vec3 direction,
      std::weak_ptr<zukou::DataOffer> data_offer) override final;

  virtual void DataDeviceLeave() override final;

  virtual void DataDeviceMotion(
      uint32_t time, glm::vec3 origin, glm::vec3 direction) override final;

  virtual void DataDeviceDrop() override final;

  virtual void Configured([[maybe_unused]] uint32_t serial) override final;

  void OnError(std::string reason);

  void RotateWithRay();

  Window(std::shared_ptr<zukou::Application> app, float radius,
      std::string host, std::string port, std::string user_id);

 private:
  std::shared_ptr<zukou::objects::IObject> focus_object_;
  std::shared_ptr<zukou::objects::IObject> dnd_focus_object_;
  std::weak_ptr<zukou::DataOffer> data_offer_;
  uint32_t dnd_enter_serial_;
  std::shared_ptr<zigen_playground::Playground> playground_;
  std::shared_ptr<Pedestal> pedestal_;
  std::shared_ptr<zukou::Application> app_;
  std::string host_;
  std::string port_;
  std::string user_id_;
  glm::vec3 ray_origin_, ray_direction_;
  glm::quat playground_quaternion_;
  bool in_control_;
};

#endif  //  ZIGEN_PLAYGROUND_MAIN_WINDOW_H
