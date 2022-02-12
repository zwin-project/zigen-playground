#include "window.h"

#include <linux/input.h>

#include <iostream>

#include "pedestal.h"

std::shared_ptr<Window> Window::Create(std::shared_ptr<zukou::Application> app,
    float radius, std::string host, std::string port, std::string user_id) {
  return std::shared_ptr<Window>(new Window(app, radius, host, port, user_id));
}

bool Window::Init() {
  auto self = shared_from_this();
  playground_ =
      zigen_playground::Playground::Create(app_, self, host_, port_, user_id_);

  playground_->Connect(zigen_playground::Playground::error_signal(),
      std::bind(&Window::OnError, std::dynamic_pointer_cast<Window>(self),
          std::placeholders::_1));

  if (!playground_->Init()) return false;

  pedestal_ = Pedestal::Create(
      app_, std::dynamic_pointer_cast<zukou::CuboidWindow>(self));

  if (!pedestal_->Init()) return false;

  return true;
}

bool Window::Frame() {
  if (in_control_) {
    RotateWithRay();
    this->ScheduleNextFrame();
  }
  bool result = false;
  if (playground_->Draw() == true) result = true;
  if (pedestal_->Draw() == true) result = true;
  return result;
}

void Window::RayLeave([[maybe_unused]] uint32_t serial) {
  if (in_control_) {
  } else {
    if (focus_object_) {
      focus_object_->RayLeave();
      focus_object_.reset();
    }
  }
}

void Window::RayMotion(uint32_t time, glm::vec3 origin, glm::vec3 direction) {
  ray_origin_ = origin;
  ray_direction_ = direction;
  if (!in_control_) {
    std::shared_ptr<zukou::objects::IObject> next_focus;
    if (pedestal_->Intersect(origin, direction) > 0) {
      next_focus = pedestal_;
    } else if (playground_->Intersect(origin, direction) > 0) {
      next_focus = playground_;
    }

    if (focus_object_ != next_focus) {
      if (focus_object_) focus_object_->RayLeave();
      if (next_focus) next_focus->RayEnter();
      focus_object_ = next_focus;
    }

    if (focus_object_) focus_object_->RayMotion(origin, direction, time);
  }
}

void Window::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  if (button == BTN_RIGHT) {
    in_control_ = pressed;
    if (in_control_) {
      if (focus_object_) focus_object_->RayLeave();
    }
    playground_->ShowFrame(in_control_);
  }

  if (focus_object_) focus_object_->RayButton(serial, time, button, pressed);
}

void Window::RayAxis(uint32_t time, uint32_t axis, float value) {
  if (focus_object_) focus_object_->RayAxis(time, axis, value);
}

void Window::RayFrame() {
  if (focus_object_) focus_object_->RayFrame();
}

void Window::RayAxisDiscrete(uint32_t axis, int32_t discrete) {
  if (focus_object_) focus_object_->RayAxisDiscrete(axis, discrete);
}

void Window::DataDeviceEnter(uint32_t serial, [[maybe_unused]] glm::vec3 origin,
    [[maybe_unused]] glm::vec3 direction,
    std::weak_ptr<zukou::DataOffer> data_offer) {
  data_offer_ = data_offer;
  dnd_enter_serial_ = serial;
}

void Window::DataDeviceLeave() {
  data_offer_.reset();
  if (dnd_focus_object_) dnd_focus_object_->DataDeviceLeave();
}

void Window::DataDeviceMotion(
    uint32_t time, glm::vec3 origin, glm::vec3 direction) {
  std::shared_ptr<zukou::objects::IObject> next_focus;
  if (pedestal_->Intersect(origin, direction) > 0) {
    next_focus = pedestal_;
  } else if (playground_->Intersect(origin, direction) > 0) {
    next_focus = playground_;
  }

  if (dnd_focus_object_ != next_focus) {
    if (dnd_focus_object_) dnd_focus_object_->DataDeviceLeave();
    if (next_focus) next_focus->DataDeviceEnter(dnd_enter_serial_, data_offer_);
    dnd_focus_object_ = next_focus;
  }

  if (dnd_focus_object_)
    dnd_focus_object_->DataDeviceMotion(time, origin, direction);
}

void Window::DataDeviceDrop() {
  if (dnd_focus_object_) dnd_focus_object_->DataDeviceDrop();
}

void Window::Configured([[maybe_unused]] uint32_t serial) {
  playground_->SetGeometry(half_size()[0], glm::vec3(), playground_quaternion_);
  pedestal_->SetGeometry(half_size()[0] + 0.05, glm::vec3(), glm::quat());
}

void Window::OnError(std::string reason) {
  std::cerr << reason << std::endl;
  app_->Terminate(EXIT_FAILURE);
}

void Window::RotateWithRay() {
  glm::vec3 model_direction =
      -glm::normalize(ray_origin_);  // consider translation if needed
  glm::vec3 axis = glm::cross(ray_direction_, model_direction);
  float force = M_PI * glm::length(axis) / 30.0f;
  glm::quat delta = glm::angleAxis(force, glm::normalize(axis));
  playground_quaternion_ = delta * playground_quaternion_;
  playground_->SetGeometry(half_size()[0], glm::vec3(), playground_quaternion_);
}

Window::Window(std::shared_ptr<zukou::Application> app, float radius,
    std::string host, std::string port, std::string user_id)
    : zukou::CuboidWindow(app, glm::vec3(radius)),
      app_(app),
      host_(host),
      port_(port),
      user_id_(user_id),
      playground_quaternion_(glm::vec3(0)),
      in_control_(false) {}
