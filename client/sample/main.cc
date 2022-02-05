#include <signal.h>
#include <sys/signalfd.h>
#include <zigen-playground.h>
#include <zukou/zukou.h>

#include <iostream>

constexpr float width = 1.0f;   // 1 m
constexpr float height = 0.5f;  // 50 cm
constexpr float depth = 0.7f;   // 70 cm

class Window : public zukou::CuboidWindow {
 public:
  static std::shared_ptr<Window> Create(std::shared_ptr<zukou::Application> app,
      glm::vec3 half_size, std::string host, std::string port,
      std::string user_id) {
    return std::shared_ptr<Window>(
        new Window(app, half_size, host, port, user_id));
  }

  bool Init() {
    auto self = shared_from_this();
    object_group_.reset(new zukou::objects::ObjectGroup());
    playground_ = zigen_playground::Playground::Create(
        app_, self, host_, port_, user_id_);

    playground_->Connect(zigen_playground::Playground::error_signal(),
        std::bind(&Window::OnError, std::dynamic_pointer_cast<Window>(self),
            std::placeholders::_1));

    if (!playground_->Init()) return false;

    object_group_->AddObject(playground_);

    return true;
  }

 private:
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
    playground_->SetGeometry(half_size(), glm::vec3(), glm::quat());
  }

  void OnError(std::string reason) {
    std::cerr << reason << std::endl;
    app_->Terminate(EXIT_FAILURE);
  }

  Window(std::shared_ptr<zukou::Application> app, glm::vec3 half_size,
      std::string host, std::string port, std::string user_id)
      : zukou::CuboidWindow(app, half_size),
        app_(app),
        host_(host),
        port_(port),
        user_id_(user_id) {}

 private:
  std::unique_ptr<zukou::objects::ObjectGroup> object_group_;
  std::shared_ptr<zigen_playground::Playground> playground_;
  std::shared_ptr<zukou::Application> app_;
  std::string host_;
  std::string port_;
  std::string user_id_;
};

class SignalEvent : public zukou::PollEvent {
 public:
  SignalEvent(std::weak_ptr<zukou::Application> app) : app_(app) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    fd_ = signalfd(-1, &mask, 0);
    op_ = EPOLL_CTL_ADD;
    epoll_event_.events = EPOLLIN;
    epoll_event_.data.ptr = this;
  }

  virtual bool Done([[maybe_unused]] struct epoll_event *ev) override final {
    auto app = app_.lock();
    app->Terminate(EXIT_SUCCESS);
    return true;
  }

  ~SignalEvent() { close(fd_); }

  std::weak_ptr<zukou::Application> app_;
};

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    std::cerr << argv[0] << " <host> <port> <user-id>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string host = argv[1];
  std::string port = argv[2];
  std::string user_id = argv[3];

  int exit_status = EXIT_FAILURE;
  auto app = zukou::Application::Create();
  glm::vec3 half_size(width / 2, height / 2, depth / 2);
  std::shared_ptr<zukou::PollEvent> sig_event(new SignalEvent(app));

  app->Connect("zigen-0");

  app->AddPollEvent(sig_event);

  auto window = Window::Create(app, half_size, host, port, user_id);
  if (!window->Init()) goto err;

  exit_status = app->Run();

err:
  std::cout << "\nexit (" << exit_status << ")" << std::endl;
  return exit_status;
}
