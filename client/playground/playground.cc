#include <zigen-playground.h>

#include <iostream>

namespace zigen_playground {

Playground::Playground(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object) {
  (void)app;
  (void)virtual_object;
}

void Playground::SetGeometry(glm::vec3 half_size, glm::vec3 position) {
  half_size_ = half_size;
  position_ = position;
}

bool Playground::Init() { return true; }

bool Playground::Draw() { return false; }

float Playground::Intersect(glm::vec3 origin, glm::vec3 direction) {
  glm::mat4 transform(1);
  transform = glm::translate(transform, position_);

  return zukou::entities::intersection::ray_obb(
      origin, direction, half_size_, transform);
}

void Playground::RayEnter() {}

void Playground::RayLeave() {}

void Playground::RayMotion(
    glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  (void)origin;
  (void)direction;
  (void)time;
}

void Playground::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  (void)serial;
  (void)time;
  (void)button;
  (void)pressed;
}

}  // namespace zigen_playground
