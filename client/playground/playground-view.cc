#include "playground-view.h"

#include <zigen-playground.h>

namespace zigen_playground {

PlaygroundView::PlaygroundView(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object) {
  (void)app;
  (void)virtual_object;
}

bool PlaygroundView::Init() { return true; }

bool PlaygroundView::Draw() { return false; }

float PlaygroundView::Intersect(glm::vec3 origin, glm::vec3 direction) {
  (void)origin;
  (void)direction;
  return -1;
}

void PlaygroundView::RayEnter() {}

void PlaygroundView::RayLeave() {}

void PlaygroundView::RayMotion(
    glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  (void)origin;
  (void)direction;
  (void)time;
}

void PlaygroundView::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  (void)serial;
  (void)time;
  (void)button;
  (void)pressed;
}

}  // namespace zigen_playground
