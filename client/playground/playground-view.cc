#include "playground-view.h"

#include <zigen-client-protocol.h>
#include <zigen-playground.h>

#include <glm/gtx/intersect.hpp>
#include <glm/gtx/quaternion.hpp>

#include "dnd-message-parser.h"

namespace zigen_playground {

PlaygroundView::PlaygroundView(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object,
    std::string remote_host, std::string remote_port)
    : app_(app),
      virtual_object_(virtual_object),
      object_group_(std::make_shared<zukou::objects::ObjectGroup>()),
      remote_host_(remote_host),
      remote_port_(remote_port) {}

bool PlaygroundView::Init() { return object_group_->Init(); }

bool PlaygroundView::Draw() {
  if (frame_) frame_->Draw();
  object_group_->Draw();
  for (auto ray_view : ray_views_) ray_view->Draw();
  return true;
}

float PlaygroundView::Intersect(glm::vec3 origin, glm::vec3 direction) {
  glm::vec3 position, norm;

  if (glm::intersectRaySphere(
          origin, direction, position_, r_, position, norm) == false)
    return -1;

  ray_distance_ = glm::length(position - origin);

  return ray_distance_;
}

void PlaygroundView::RayEnter() {}

void PlaygroundView::RayLeave() { object_group_->RayLeave(); }

void PlaygroundView::RayMotion(
    glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  ray_origin_ = origin;
  ray_direction_ = direction;

  object_group_->RayMotion(origin, direction, time);

  this->NotifyRayChange();
}

void PlaygroundView::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  object_group_->RayButton(serial, time, button, pressed);
}

void PlaygroundView::RayAxis(uint32_t time, uint32_t axis, float value) {
  object_group_->RayAxis(time, axis, value);
  this->NotifyRayChange();
}

void PlaygroundView::RayFrame() { object_group_->RayFrame(); }

void PlaygroundView::RayAxisDiscrete(uint32_t axis, int32_t discrete) {
  object_group_->RayAxisDiscrete(axis, discrete);
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

    glm::vec3 intersection_point, norm;

    glm::mat4 transform(1);
    transform = glm::translate(transform, position_);
    transform = transform * glm::toMat4(quaternion_);

    if (glm::intersectRaySphere(last_data_device_origin_,
            last_data_device_direction_, position_, r_, intersection_point,
            norm) == false)
      return;

    float ray_distance =
        glm::length(intersection_point - last_data_device_origin_);

    auto position = last_data_device_origin_ +
                    last_data_device_direction_ * (ray_distance + 0.04f);

    dnd_new_resource_callback(new_resource_message->resource_type(),
        glm::vec3(glm::inverse(transform) * glm::vec4(position, 1.0)));
  }
}

void PlaygroundView::SetGeometry(
    float r, glm::vec3 position, glm::quat quaternion) {
  r_ = r;
  position_ = position;
  quaternion_ = quaternion;

  for (auto cuboid_view : cuboid_views_)
    cuboid_view->SetGeometry(position, quaternion);
  for (auto sphere_view : sphere_views_)
    sphere_view->SetGeometry(position, quaternion);
  for (auto ray_view : ray_views_) ray_view->SetGeometry(position, quaternion);

  if (frame_) {
    frame_->set_position(position);
    frame_->set_quaternion(quaternion);
    frame_->set_radius(r);
  }

  this->NotifyRayChange();
}

void PlaygroundView::ShowFrame(bool show) {
  if (show) {
    frame_ = std::make_shared<zukou::primitives::FrameSphere>(
        app_, virtual_object_, 10);
    frame_->Init();
    frame_->set_frame_color(glm::vec4(0.0, 1.0, 1.0, 1.0));
    frame_->set_position(position_);
    frame_->set_quaternion(quaternion_);
    frame_->set_radius(r_);
  } else {
    frame_.reset();
  }
  virtual_object_->ScheduleNextFrame();
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
      auto sphere_view = std::make_shared<SphereView>(app_, virtual_object_,
          sphere, remote_host_, remote_port_, dnd_new_texture_callback,
          update_sphere_geom_callback);
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

void PlaygroundView::AddResource(std::shared_ptr<model::Resource> resource) {
  if (resource->type == "cuboid") {
    auto cuboid = std::dynamic_pointer_cast<model::Cuboid>(resource);
    auto cuboid_view =
        std::make_shared<CuboidView>(app_, virtual_object_, cuboid);
    cuboid_views_.push_back(cuboid_view);
    object_group_->AddObject(cuboid_view);
    cuboid_view->Init();
    cuboid_view->SetGeometry(position_, quaternion_);
  } else if (resource->type == "sphere") {
    auto sphere = std::dynamic_pointer_cast<model::Sphere>(resource);
    auto sphere_view = std::make_shared<SphereView>(app_, virtual_object_,
        sphere, remote_host_, remote_port_, dnd_new_texture_callback,
        update_sphere_geom_callback);
    sphere_views_.push_back(sphere_view);
    object_group_->AddObject(sphere_view);
    sphere_view->Init();
    sphere_view->SetGeometry(position_, quaternion_);
  }

  virtual_object_->ScheduleNextFrame();
}

void PlaygroundView::UpdateTexture(
    uint64_t resource_id, std::string texture_url) {
  for (auto sphere_view : sphere_views_) {
    if (sphere_view->GetId() == resource_id)
      sphere_view->SetTexture(texture_url);
  }
}

void PlaygroundView::UpdateGeom(std::shared_ptr<model::Resource> resource) {
  if (resource->type == "sphere") {
    auto sphere = std::dynamic_pointer_cast<model::Sphere>(resource);
    for (auto sphere_view : sphere_views_) {
      if (sphere_view->GetId() == sphere->id) {
        sphere_view->UpdateSphereGeometry(sphere);
      }
    }
  }
}

void PlaygroundView::RemoveRay(uint64_t client_id) {
  auto it = ray_views_.begin();
  while (it != ray_views_.end()) {
    if ((*it)->GetId() == client_id) {
      ray_views_.erase(it);
      return;
    }
  }
  virtual_object_->ScheduleNextFrame();
}

void PlaygroundView::MoveRay(std::shared_ptr<model::Ray> ray) {
  for (auto ray_view : ray_views_) {
    if (ray_view->GetId() == ray->client_id) {
      ray_view->UpdateRay(ray);
      virtual_object_->ScheduleNextFrame();
      return;
    }
  }

  auto ray_view = std::make_shared<RayView>(app_, virtual_object_, ray);
  ray_view->Init();
  ray_view->SetGeometry(position_, quaternion_);
  ray_views_.push_back(ray_view);
  virtual_object_->ScheduleNextFrame();
}

void PlaygroundView::NotifyRayChange() {
  glm::vec3 origin_tip = ray_origin_ + ray_direction_ * ray_distance_;
  float target_distance = object_group_->Intersect(ray_origin_, ray_direction_);
  target_distance =
      target_distance <= 0 ? ray_distance_ + 0.5 : target_distance;
  glm::vec3 target_tip = ray_origin_ + ray_direction_ * target_distance;

  glm::mat4 transform(1);
  transform = glm::translate(transform, position_);
  transform = transform * glm::toMat4(quaternion_);
  transform = glm::inverse(transform);

  origin_tip = transform * glm::vec4(origin_tip, 1.0f);
  target_tip = transform * glm::vec4(target_tip, 1.0f);

  update_ray_callback(origin_tip, target_tip);
}

}  // namespace zigen_playground
