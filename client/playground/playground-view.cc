#include "playground-view.h"

#include <zigen-playground.h>

namespace zigen_playground {

PlaygroundView::PlaygroundView(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object)
    : app_(app),
      virtual_object_(virtual_object),
      object_group_(std::make_shared<zukou::objects::ObjectGroup>()) {}

bool PlaygroundView::Init() { return object_group_->Init(); }

bool PlaygroundView::Draw() { return object_group_->Draw(); }

float PlaygroundView::Intersect(glm::vec3 origin, glm::vec3 direction) {
  (void)origin;
  (void)direction;
  glm::mat4 transform(1);
  transform = glm::translate(transform, position_);

  return zukou::entities::intersection::ray_obb(
             origin, direction, half_size_, transform) < -1;
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

void PlaygroundView::SetGeometry(
    glm::vec3 half_size, glm::vec3 position, glm::quat quaternion) {
  half_size_ = half_size;
  position_ = position;
  quaternion_ = quaternion;

  for (auto cuboid_view : cuboid_views_)
    cuboid_view->SetGeometry(position, quaternion);
}

void PlaygroundView::Sync(
    std::vector<std::shared_ptr<model::Resource>> resources) {
  object_group_ = std::make_shared<zukou::objects::ObjectGroup>();
  cuboid_views_.clear();
  for (auto resource : resources) {
    if (resource->type == "cuboid") {
      auto cuboid = std::dynamic_pointer_cast<model::Cuboid>(resource);
      auto cuboid_view =
          std::make_shared<CuboidView>(app_, virtual_object_, cuboid);
      cuboid_views_.push_back(cuboid_view);
      object_group_->AddObject(cuboid_view);
    }
  }
  object_group_->Init();

  for (auto cuboid_view : cuboid_views_) {
    cuboid_view->Init();
    cuboid_view->SetGeometry(position_, quaternion_);
  }

  virtual_object_->ScheduleNextFrame();
}

}  // namespace zigen_playground
