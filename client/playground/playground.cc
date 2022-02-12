#include <zigen-playground.h>

#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "client.h"
#include "playground-view.h"

namespace zigen_playground {

std::shared_ptr<Playground> Playground::Create(
    std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object,
    std::string remote_host, std::string remote_port, std::string remote_path) {
  return std::shared_ptr<Playground>(new Playground(
      app, virtual_object, remote_host, remote_port, remote_path));
}

Playground::Playground(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object,
    std::string remote_host, std::string remote_port, std::string user_id)
    : app_(app),
      virtual_object_(virtual_object),
      remote_host_(remote_host),
      remote_port_(remote_port),
      user_id_(user_id) {}

void Playground::SetGeometry(
    float r, glm::vec3 position, glm::quat quaternion) {
  view_->SetGeometry(r, position, quaternion);
}

void Playground::ShowFrame(bool show) { view_->ShowFrame(show); }

bool Playground::Init() {
  auto self = shared_from_this();
  view_.reset(
      new PlaygroundView(app_, virtual_object_, remote_host_, remote_port_));
  client_ = Client::Create(
      app_, remote_host_, remote_port_, std::string("/ws?id=") + user_id_);
  client_->ConnectNoopEventSignal(std::bind(&Playground::NoopEvent, self));
  client_->ConnectSyncEventSignal(
      std::bind(&Playground::SyncEvent, self, std::placeholders::_1));
  client_->ConnectNewResourceEventSignal(
      std::bind(&Playground::NewResourceEvent, self, std::placeholders::_1));
  client_->ConnectNewTextureEventSignal(std::bind(&Playground::NewTextureEvent,
      self, std::placeholders::_1, std::placeholders::_2));
  client_->ConnectUpdateGeomEventSignal(
      std::bind(&Playground::UpdataGeomEvent, self, std::placeholders::_1));
  client_->ConnectErrorSignal(
      std::bind(&Playground::ClientErrorEvent, self, std::placeholders::_1));

  view_->dnd_new_resource_callback = std::bind(&Playground::DndNewResource,
      self, std::placeholders::_1, std::placeholders::_2);
  view_->dnd_new_texture_callback = std::bind(&Playground::DndNewTexture, self,
      std::placeholders::_1, std::placeholders::_2);
  view_->update_sphere_geom_callback =
      std::bind(&Playground::UpdateSphereGeom, self, std::placeholders::_1);

  if (client_->Connect() == false) return false;
  client_->SyncRequest();
  return view_->Init();
}

bool Playground::Draw() {
  view_->Draw();
  return true;
}

float Playground::Intersect(glm::vec3 origin, glm::vec3 direction) {
  return view_->Intersect(origin, direction);
}

void Playground::RayEnter() { view_->RayEnter(); }

void Playground::RayLeave() { view_->RayLeave(); }

void Playground::RayMotion(
    glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  view_->RayMotion(origin, direction, time);
}

void Playground::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  view_->RayButton(serial, time, button, pressed);
}

void Playground::RayAxis(uint32_t time, uint32_t axis, float value) {
  view_->RayAxis(time, axis, value);
}

void Playground::RayFrame() { view_->RayFrame(); }

void Playground::RayAxisDiscrete(uint32_t axis, int32_t discrete) {
  view_->RayAxisDiscrete(axis, discrete);
}

void Playground::DataDeviceEnter(
    uint32_t serial, std::weak_ptr<zukou::DataOffer> data_offer) {
  view_->DataDeviceEnter(serial, data_offer);
}

void Playground::DataDeviceLeave() { view_->DataDeviceLeave(); }

void Playground::DataDeviceMotion(
    uint32_t time, glm::vec3 origin, glm::vec3 direction) {
  view_->DataDeviceMotion(time, origin, direction);
}

void Playground::DataDeviceDrop() { view_->DataDeviceDrop(); }

void Playground::NoopEvent() { std::cerr << "[event] noop" << std::endl; }

void Playground::SyncEvent(
    std::vector<std::shared_ptr<model::Resource>> resources) {
  std::cerr << "[event] sync" << std::endl;
  view_->Sync(resources);
}

void Playground::NewResourceEvent(std::shared_ptr<model::Resource> resource) {
  std::cerr << "[event] new resource" << std::endl;
  view_->AddResource(resource);
}

void Playground::NewTextureEvent(
    uint64_t resource_id, std::string texture_url) {
  std::cerr << "[event] new texture" << std::endl;
  view_->UpdateTexture(resource_id, texture_url);
}

void Playground::UpdataGeomEvent(std::shared_ptr<model::Resource> resource) {
  std::cerr << "[event] update geom" << std::endl;
  view_->UpdateGeom(resource);
}

void Playground::Connect([[maybe_unused]] const error_signal signal,
    const std::function<void(std::string)> &slot) {
  error_callback_ = slot;
}

void Playground::ClientErrorEvent(std::string reason) {
  if (error_callback_) error_callback_(reason);
}

void Playground::DndNewResource(std::string resource_type, glm::vec3 position) {
  std::cerr << "[dnd event] new resource " << resource_type
            << " pos: " << glm::to_string(position) << std::endl;
  client_->NewResourceRequest(resource_type, position);
}

void Playground::DndNewTexture(uint64_t resource_id, std::string url) {
  std::cerr << "[dnd event] new texture id: " << resource_id << ", url: " << url
            << std::endl;
  client_->NewTextureRequest(resource_id, url);
}

void Playground::UpdateSphereGeom(std::shared_ptr<model::Sphere> sphere) {
  std::cerr << "[action event] update sphere geom" << std::endl;
  client_->UpdateSphereGeom(sphere);
}

}  // namespace zigen_playground
