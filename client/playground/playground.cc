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

void Playground::SetGeometry(glm::vec3 half_size, glm::vec3 position) {
  half_size_ = half_size;
  position_ = position;
}

bool Playground::Init() {
  auto self = shared_from_this();
  view_.reset(new PlaygroundView(app_, virtual_object_));
  client_ = Client::Create(
      app_, remote_host_, remote_port_, std::string("/ws?id=") + user_id_);
  client_->ConnectNoopEventSignal(std::bind(&Playground::NoopEvent, self));
  client_->ConnectSyncEventSignal(
      std::bind(&Playground::SyncEvent, self, std::placeholders::_1));
  client_->ConnectErrorSignal(
      std::bind(&Playground::ClientErrorEvent, self, std::placeholders::_1));

  if (client_->Connect() == false) return false;
  client_->SyncRequest();
  return view_->Init();
}

bool Playground::Draw() { return view_->Draw(); }

float Playground::Intersect(glm::vec3 origin, glm::vec3 direction) {
  glm::mat4 transform(1);
  transform = glm::translate(transform, position_);

  if (zukou::entities::intersection::ray_obb(
          origin, direction, half_size_, transform) < -1)
    return false;

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

void Playground::NoopEvent() {
  std::cerr << "Playground Noop Action!!" << std::endl;
}

void Playground::SyncEvent(
    std::vector<std::shared_ptr<model::Resource>> resources) {
  std::cerr << "Playground Sync Action!!" << std::endl;
  for (auto resource : resources) {
    std::cerr << resource->type << ": ";
    if (resource->type == "cuboid") {
      auto cuboid = std::dynamic_pointer_cast<model::Cuboid>(resource);
      std::cerr << glm::to_string(cuboid->position) << ", "
                << glm::to_string(cuboid->half_size);
    }
    std::cerr << std::endl;
  }
}

void Playground::Connect([[maybe_unused]] const error_signal signal,
    const std::function<void(std::string)> &slot) {
  error_callback_ = slot;
}

void Playground::ClientErrorEvent(std::string reason) {
  if (error_callback_) error_callback_(reason);
}

}  // namespace zigen_playground
