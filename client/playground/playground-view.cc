#include "playground-view.h"

#include <zigen-client-protocol.h>
#include <zigen-playground.h>

#include <glm/gtx/quaternion.hpp>

#include "dnd-message-parser.h"

namespace zigen_playground {

PlaygroundView::PlaygroundView(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object)
    : app_(app),
      virtual_object_(virtual_object),
      object_group_(std::make_shared<zukou::objects::ObjectGroup>()) {}

bool PlaygroundView::Init() { return object_group_->Init(); }

bool PlaygroundView::Draw() { return object_group_->Draw(); }

float PlaygroundView::Intersect(glm::vec3 origin, glm::vec3 direction) {
  glm::mat4 transform(1);
  transform = glm::translate(transform, position_);
  transform = transform * glm::toMat4(quaternion_);

  return zukou::entities::intersection::ray_obb(
      origin, direction, half_size_, transform);
}

void PlaygroundView::RayEnter() {}

void PlaygroundView::RayLeave() { object_group_->RayLeave(); }

void PlaygroundView::RayMotion(
    glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  object_group_->RayMotion(origin, direction, time);
}

void PlaygroundView::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  object_group_->RayButton(serial, time, button, pressed);
}

void PlaygroundView::DataDeviceEnter(
    uint32_t serial, std::weak_ptr<zukou::DataOffer> data_offer) {
  if (auto offer = data_offer.lock()) {
    offer->SetActions(ZGN_DATA_DEVICE_MANAGER_DND_ACTION_MOVE |
                          ZGN_DATA_DEVICE_MANAGER_DND_ACTION_COPY,
        ZGN_DATA_DEVICE_MANAGER_DND_ACTION_COPY);
  }
  data_offer_ = data_offer;
  data_device_enter_serial_ = serial;
  object_group_->DataDeviceEnter(serial, data_offer);
}

void PlaygroundView::DataDeviceLeave() {
  data_offer_.reset();
  object_group_->DataDeviceLeave();
}

void PlaygroundView::DataDeviceMotion(
    uint32_t time, glm::vec3 origin, glm::vec3 direction) {
  last_data_device_origin_ = origin;
  last_data_device_direction_ = direction;
  if (auto data_offer = data_offer_.lock()) {
    for (auto mime_type : data_offer->mime_types()) {
      if (mime_type == "text/plain") {
        data_offer->Accept(data_device_enter_serial_, mime_type);
        break;
      }
    }
  }
  object_group_->DataDeviceMotion(time, origin, direction);
}

void PlaygroundView::DataDeviceDrop() {
  if (auto data_offer = data_offer_.lock()) {
    for (auto mime_type : data_offer->mime_types()) {
      if (mime_type == "text/plain") {
        data_offer->Receive(mime_type, std::bind(&PlaygroundView::TextDropped,
                                           this, std::placeholders::_1));
        break;
      }
    }
  }
  object_group_->DataDeviceDrop();
}

void PlaygroundView::TextDropped(int fd) {
  char buf[100];
  std::string str;
  DndMessageParser parser;

  while (true) {
    int size = read(fd, buf, 100);
    if (size <= 0) break;
    std::string fragment(buf, size);
    str += fragment;
  }

  auto message = parser.Parse(str);

  if (message->type() == DndMessageType::kError) return;

  if (message->type() == DndMessageType::kNewResource) {
    auto new_resource_message =
        std::dynamic_pointer_cast<DndNewResourceMessage>(message);

    glm::mat4 transform(1);
    transform = glm::translate(transform, position_);
    transform = transform * glm::toMat4(quaternion_);

    float ray_distance =
        zukou::entities::intersection::ray_obb(last_data_device_origin_,
            last_data_device_direction_, half_size_, transform);

    auto position = last_data_device_origin_ +
                    last_data_device_direction_ * (ray_distance + 0.04f);

    dnd_new_resource_callback(new_resource_message->resource_type(),
        glm::vec3(glm::inverse(transform) * glm::vec4(position, 1.0)));
  }
}

void PlaygroundView::SetGeometry(
    glm::vec3 half_size, glm::vec3 position, glm::quat quaternion) {
  half_size_ = half_size;
  position_ = position;
  quaternion_ = quaternion;

  for (auto cuboid_view : cuboid_views_)
    cuboid_view->SetGeometry(position, quaternion);
  for (auto sphere_view : sphere_views_)
    sphere_view->SetGeometry(position, quaternion);
}

void PlaygroundView::Sync(
    std::vector<std::shared_ptr<model::Resource>> resources) {
  object_group_ = std::make_shared<zukou::objects::ObjectGroup>();
  cuboid_views_.clear();
  sphere_views_.clear();
  for (auto resource : resources) {
    if (resource->type == "cuboid") {
      auto cuboid = std::dynamic_pointer_cast<model::Cuboid>(resource);
      auto cuboid_view =
          std::make_shared<CuboidView>(app_, virtual_object_, cuboid);
      cuboid_views_.push_back(cuboid_view);
      object_group_->AddObject(cuboid_view);
    } else if (resource->type == "sphere") {
      auto sphere = std::dynamic_pointer_cast<model::Sphere>(resource);
      auto sphere_view =
          std::make_shared<SphereView>(app_, virtual_object_, sphere);
      sphere_views_.push_back(sphere_view);
      object_group_->AddObject(sphere_view);
    }
  }
  object_group_->Init();

  for (auto cuboid_view : cuboid_views_) {
    cuboid_view->Init();
    cuboid_view->SetGeometry(position_, quaternion_);
  }

  for (auto sphere_view : sphere_views_) {
    sphere_view->Init();
    sphere_view->SetGeometry(position_, quaternion_);
  }

  virtual_object_->ScheduleNextFrame();
}

}  // namespace zigen_playground
