#include <zigen-playground.h>
#include <zukou/zukou.h>

constexpr float width = 1.0f;   // 1 m
constexpr float height = 0.5f;  // 50 cm
constexpr float depth = 0.7f;   // 70 cm

class Window : public zukou::CuboidWindow {
 public:
  static std::shared_ptr<Window> Create(
      std::shared_ptr<zukou::Application> app, glm::vec3 half_size) {
    return std::shared_ptr<Window>(new Window(app, half_size));
  }

  bool Init() {
    auto self = shared_from_this();
    object_group_.reset(new zukou::objects::ObjectGroup());
    playground_.reset(new zigen_playground::Playground(app_, self));

    if (!playground_->Init()) return false;

    object_group_->AddObject(playground_);

    return true;
  }

 protected:
  virtual bool Frame() final { return object_group_->Draw(); }

  virtual void RayLeave([[maybe_unused]] uint32_t serial) final {
    object_group_->RayLeave();
  }

  virtual void RayMotion(
      uint32_t time, glm::vec3 origin, glm::vec3 direction) final {
    object_group_->RayMotion(origin, direction, time);
  }

  virtual void RayButton(
      uint32_t serial, uint32_t time, uint32_t button, bool pressed) final {
    object_group_->RayButton(serial, time, button, pressed);
  }

  virtual void Configured([[maybe_unused]] uint32_t serial) final {
    playground_->SetGeometry(half_size(), glm::vec3());
  }

 private:
  Window(std::shared_ptr<zukou::Application> app, glm::vec3 half_size)
      : zukou::CuboidWindow(app, half_size) {}

 private:
  std::unique_ptr<zukou::objects::ObjectGroup> object_group_;
  std::shared_ptr<zigen_playground::Playground> playground_;
};

int main() {
  auto app = zukou::Application::Create();
  glm::vec3 half_size(width / 2, height / 2, depth / 2);

  app->Connect("zigen-0");

  auto window = Window::Create(app, half_size);
  if (!window->Init()) return EXIT_FAILURE;

  if (!app->Run()) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
