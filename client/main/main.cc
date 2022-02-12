#include <signal.h>
#include <sys/signalfd.h>
#include <zigen-playground.h>
#include <zukou/zukou.h>

#include <iostream>

#include "window.h"

constexpr float radius = 1.0f;  // 1 m

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
  std::shared_ptr<zukou::PollEvent> sig_event(new SignalEvent(app));

  app->Connect("zigen-0");

  app->AddPollEvent(sig_event);

  auto window = Window::Create(app, radius, host, port, user_id);
  if (!window->Init()) goto err;

  exit_status = app->Run();

err:
  std::cout << "\nexit (" << exit_status << ")" << std::endl;
  return exit_status;
}
