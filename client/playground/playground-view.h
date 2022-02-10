#ifndef ZIGEN_PLAYGROUND_PLAYGROUND_VIEW_H
#define ZIGEN_PLAYGROUND_PLAYGROUND_VIEW_H

#include <zigen-playground.h>

#include <functional>
#include <memory>

#include "cuboid-view.h"
#include "sphere-view.h"

namespace zigen_playground {

class PlaygroundView : public zukou::objects::IObject {
 public:
  PlaygroundView(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::string remote_host, std::string remote_port);

  virtual bool Init() override final;
  virtual bool Draw() override final;
  virtual float Intersect(glm::vec3 origin, glm::vec3 direction) override final;

  virtual void RayEnter() override final;
  virtual void RayLeave() override final;
  virtual void RayMotion(
      glm::vec3 origin, glm::vec3 direction, uint32_t time) override final;
  virtual void RayButton(uint32_t serial, uint32_t time, uint32_t button,
      bool pressed) override final;
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

  void TextDropped(int fd);

  void SetGeometry(
      glm::vec3 half_size, glm::vec3 position, glm::quat quaternion);

  void Sync(std::vector<std::shared_ptr<model::Resource>> resources);

  void AddResource(std::shared_ptr<model::Resource> resource);
  void UpdateTexture(uint64_t resource_id, std::string texture_url);
  void UpdateGeom(std::shared_ptr<model::Resource> resource);

  // callbacks
  std::function<void(std::string resource_type, glm::vec3 position)>
      dnd_new_resource_callback;

  std::function<void(uint64_t resource_id, std::string url)>
      dnd_new_texture_callback;

  std::function<void(std::shared_ptr<model::Sphere>)>
      update_sphere_geom_callback;

 private:
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::VirtualObject> virtual_object_;
  std::shared_ptr<zukou::objects::ObjectGroup> object_group_;
  std::vector<std::shared_ptr<CuboidView>> cuboid_views_;
  std::vector<std::shared_ptr<SphereView>> sphere_views_;
  glm::vec3 half_size_;
  glm::vec3 position_;
  glm::quat quaternion_;

  glm::vec3 last_data_device_origin_;
  glm::vec3 last_data_device_direction_;
  std::weak_ptr<zukou::DataOffer> data_offer_;
  uint32_t data_device_enter_serial_;

  std::string remote_host_;
  std::string remote_port_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_PLAYGROUND_VIEW_H
