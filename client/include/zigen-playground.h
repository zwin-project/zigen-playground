#ifndef ZIGEN_PLAYGROUND_H
#define ZIGEN_PLAYGROUND_H

#include <zukou/zukou.h>

#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace zigen_playground {

namespace model {
class Resource;
class Sphere;
}  // namespace model

class PlaygroundView;
class Client;

class Playground : public std::enable_shared_from_this<Playground>,
                   public zukou::objects::IObject {
 public:
  static std::shared_ptr<Playground> Create(
      std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::string remote_host, std::string remote_port, std::string user_id);

 public:
  void SetGeometry(float r, glm::vec3 position, glm::quat quaternion);
  void ShowFrame(bool show);

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

  struct error_signal {};

  void Connect(
      const error_signal, const std::function<void(std::string)> &slot);

 private:
  Playground(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::string remote_host, std::string remote_port, std::string user_id);

  void NoopEvent();
  void SyncEvent(std::vector<std::shared_ptr<model::Resource>> resources);
  void NewResourceEvent(std::shared_ptr<model::Resource> resource);
  void NewTextureEvent(uint64_t resource_id, std::string url);
  void UpdataGeomEvent(std::shared_ptr<model::Resource> resource);
  void ClientErrorEvent(std::string reason);

  void DndNewResource(std::string resource_type, glm::vec3 position);

  void DndNewTexture(uint64_t resource_id, std::string url);

  void UpdateSphereGeom(std::shared_ptr<model::Sphere> sphere);

 private:
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::VirtualObject> virtual_object_;
  std::shared_ptr<PlaygroundView> view_;
  std::shared_ptr<Client> client_;
  std::function<void(std::string)> error_callback_;
  std::string remote_host_;
  std::string remote_port_;
  std::string user_id_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_H
