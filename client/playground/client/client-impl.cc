#include "client-impl.h"

#include <fcntl.h>
#include <unistd.h>

#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/read.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "message-parser.h"
#include "message.h"
#include "message/error.h"
#include "message/new-resource-event.h"
#include "message/new-resource-request.h"
#include "message/new-texture-event.h"
#include "message/new-texture-request.h"
#include "message/noop-event.h"
#include "message/sync-event.h"
#include "message/sync-request.h"
#include "message/update-geom-event.h"
#include "message/update-geom-request.h"
#include "message/update-ray-event.h"
#include "message/update-ray-request.h"
#include "messaging.h"

namespace zigen_playground {

class ClientEventPollEvent : public zukou::PollEvent {
 public:
  ClientEventPollEvent(std::shared_ptr<Client::Impl> client_pimpl)
      : client_pimpl_(client_pimpl) {
    fd_ = fcntl(client_pimpl->event_pipe_[0], F_DUPFD_CLOEXEC, 0);
    op_ = EPOLL_CTL_ADD;
    if (fd_ < 0) throw "failed to create fd";

    epoll_event_.events = EPOLLIN;
  }

  virtual bool Done([[maybe_unused]] struct epoll_event *ev) override {
    client_pimpl_.lock()->OnEvent();
    return false;
  }

  ~ClientEventPollEvent() { close(fd_); }

 private:
  std::weak_ptr<Client::Impl> client_pimpl_;
};

Client::Impl::Impl(std::shared_ptr<zukou::Application> app,
    std::string remote_host, std::string remote_port, std::string remote_path)
    : app_(app),
      remote_host_(remote_host),
      remote_port_(remote_port),
      remote_path_(remote_path) {
  handling_event_ = message::Action::kUnset;
  event_pipe_[0] = -1;
  event_pipe_[1] = -1;
  request_pipe_[0] = -1;
  request_pipe_[1] = -1;
}

Client::Impl::~Impl() {
  ioc_.stop();
  client_thread_.join();
  ioc_.reset();

  if (event_pipe_[0] > 0) close(event_pipe_[0]);
  if (event_pipe_[1] > 0) close(event_pipe_[1]);
  if (request_pipe_[0] > 0) close(request_pipe_[0]);
  if (request_pipe_[1] > 0) close(request_pipe_[1]);
}

bool Client::Impl::Connect() {
  auto self = shared_from_this();

  if (pipe2(event_pipe_, O_CLOEXEC | O_NONBLOCK) == -1) return false;
  if (pipe2(request_pipe_, O_CLOEXEC | O_NONBLOCK) == -1) return false;

  client_event_poll_event_.reset(new ClientEventPollEvent(self));
  app_->AddPollEvent(client_event_poll_event_);

  client_thread_.interrupt();
  client_thread_ = boost::thread(&Impl::Run, this);
  return true;
}

void Client::Impl::SyncRequest() {
  auto request = std::make_shared<message::SyncRequest>();
  NotifyRequest(request);
}

void Client::Impl::NewResourceRequest(
    std::string resource_type, glm::vec3 position) {
  auto request =
      std::make_shared<message::NewResourceRequest>(resource_type, position);
  NotifyRequest(request);
}

void Client::Impl::NewTextureRequest(uint64_t resource_id, std::string url) {
  auto request = std::make_shared<message::NewTextureRequest>(resource_id, url);
  NotifyRequest(request);
}

void Client::Impl::UpdateSphereGeom(std::shared_ptr<model::Sphere> sphere) {
  auto request = std::make_shared<message::UpdateGeomRequest>(sphere);
  NotifyRequest(request);
}

void Client::Impl::UpdateRay(glm::vec3 origin, glm::vec3 target) {
  auto request = std::make_shared<message::UpdateRayRequest>(origin, target);
  NotifyRequest(request);
}

void Client::Impl::OnEvent() {
  char buffer[1000];
  int ret = read(event_pipe_[0], buffer, sizeof(buffer));

  if (ret == 0) {
    close(event_pipe_[0]);
    event_pipe_[0] = -1;
    return;
  };

  event_message_buffer_.Push(buffer, ret);

  while (true) {
    if (handling_event_ == message::Action::kUnset) {
      if (event_message_buffer_.Size() < sizeof(message::Action)) return;
      message::Action action;
      event_message_buffer_.Pop(
          reinterpret_cast<char *>(&action), sizeof(message::Action));
      handling_event_ = action;
      event_message_size_ = 0;
    } else if (event_message_size_ == 0) {
      if (event_message_buffer_.Size() < sizeof(uint32_t)) return;
      uint32_t message_size;
      event_message_buffer_.Pop(
          reinterpret_cast<char *>(&message_size), sizeof(uint32_t));
      event_message_size_ = message_size;
    } else {
      if (event_message_buffer_.Size() < event_message_size_) return;
      void *data = malloc(event_message_size_);
      event_message_buffer_.Pop(
          reinterpret_cast<char *>(data), event_message_size_);

      if (handling_event_ == message::Action::kNoopEvent) {
        message::NoopEvent().Load(data);
        noop_event_signal();
      } else if (handling_event_ == message::Action::kError) {
        auto err = message::Error();
        err.Load(data);
        error_signal(err.reason());
      } else if (handling_event_ == message::Action::kSyncEvent) {
        auto sync = message::SyncEvent();
        sync.Load(data);
        auto resources = sync.GetResources();
        sync_event_signal(resources);
      } else if (handling_event_ == message::Action::kNewResourceEvent) {
        auto new_resource_event = message::NewResourceEvent();
        new_resource_event.Load(data);
        auto resource = new_resource_event.GetResource();
        if (resource) new_resource_event_signal(resource);
      } else if (handling_event_ == message::Action::kNewTextureEvent) {
        auto new_texture_event = message::NewTextureEvent();
        new_texture_event.Load(data);
        uint64_t resource_id = new_texture_event.GetResourceId();
        std::string texture_url = new_texture_event.GetTextureUrl();
        new_texture_event_signal(resource_id, texture_url);
      } else if (handling_event_ == message::Action::kUpdateGeomEvent) {
        auto update_geom_event = message::UpdateGeomEvent();
        update_geom_event.Load(data);
        auto resource = update_geom_event.GetResource();
        update_geom_event_signal(resource);
      } else if (handling_event_ == message::Action::kUpdateRayEvent) {
        auto update_ray_event = message::UpdateRayEvent();
        update_ray_event.Load(data);
        auto type = update_ray_event.GetType();
        auto ray = update_ray_event.GetRay();
        if (type == "destroy") {
          remove_ray_signal(ray->client_id);
        } else if (type == "move") {
          move_ray_signal(ray);
        }
      }

      free(data);
      handling_event_ = message::Action::kUnset;
    }
  }
}

void Client::Impl::NotifyRequest(
    std::shared_ptr<message::IRequestMessage> request) {
  message::Action action = request->GetAction();

  if (write(request_pipe_[1], &action, sizeof(message::Action)) !=
      sizeof(message::Action)) {
    std::cerr << "write miss";
  }

  uint32_t size = request->GetSize();
  if (write(request_pipe_[1], &size, sizeof(uint32_t)) != sizeof(uint32_t)) {
    std::cerr << "write miss";
  };

  request->WriteTo(request_pipe_[1]);
  fsync(request_pipe_[1]);
}

/* in worker thread */
void Client::Impl::Run() {
  auto self = shared_from_this();
  auto messaging = Messaging::Create(ioc_);
  boost::asio::posix::stream_descriptor stream_in(ioc_, request_pipe_[0]);

  messaging->OnNewEvent.connect(
      boost::signals2::signal<void(std::string)>::slot_type(
          &Client::Impl::OnNewEvent, this, _1)
          .track_foreign(self));

  messaging->OnFail.connect(
      boost::signals2::signal<void(std::string)>::slot_type(
          &Client::Impl::OnMessagingFail, this, _1)
          .track_foreign(self));

  messaging->Run(remote_host_, remote_port_, remote_path_);

  boost::asio::streambuf input_buffer;
  message::Action handling_request = message::Action::kUnset;
  uint32_t request_message_size;
  Fifo<char> request_message_buffer;

  std::function<void(
      boost::beast::error_code error_code, std::size_t bytes_transferred)>
      processor = [&](boost::beast::error_code error_code,
                      std::size_t bytes_transferred) {
        if (error_code) {
          this->SendMessageToMainThread(std::make_shared<message::Error>(
              "request: " + error_code.message()));
          return;
        }

        request_message_buffer.Push(
            (char *)(input_buffer.data().data()), bytes_transferred);

        input_buffer.consume(bytes_transferred);

        while (true) {
          if (handling_request == message::Action::kUnset) {
            if (request_message_buffer.Size() < sizeof(message::Action)) break;
            message::Action action;
            request_message_buffer.Pop(
                reinterpret_cast<char *>(&action), sizeof(message::Action));
            handling_request = action;
            request_message_size = 0;
          } else if (request_message_size == 0) {
            if (request_message_buffer.Size() < sizeof(uint32_t)) break;
            uint32_t message_size;
            request_message_buffer.Pop(
                reinterpret_cast<char *>(&message_size), sizeof(uint32_t));
            request_message_size = message_size;
          } else {
            if (request_message_buffer.Size() < request_message_size) break;
            void *data = malloc(request_message_size);
            request_message_buffer.Pop(
                reinterpret_cast<char *>(data), request_message_size);

            if (handling_request == message::Action::kSyncRequest) {
              auto sync_request = message::SyncRequest();
              sync_request.Load(data);
              messaging->Write(sync_request.ToJson());
            } else if (handling_request ==
                       message::Action::kNewResourceRequest) {
              auto new_resource_request = message::NewResourceRequest();
              new_resource_request.Load(data);
              messaging->Write(new_resource_request.ToJson());
            } else if (handling_request ==
                       message::Action::kNewTextureRequest) {
              auto new_texture_request = message::NewTextureRequest();
              new_texture_request.Load(data);
              messaging->Write(new_texture_request.ToJson());
            } else if (handling_request ==
                       message::Action::kUpdateGeomRequest) {
              auto update_sphere_geom_request = message::UpdateGeomRequest();
              update_sphere_geom_request.Load(data);
              messaging->Write(update_sphere_geom_request.ToJson());
            } else if (handling_request == message::Action::kUpdateRayRequest) {
              auto update_ray_request = message::UpdateRayRequest();
              update_ray_request.Load(data);
              messaging->Write(update_ray_request.ToJson());
            }

            free(data);
            handling_request = message::Action::kUnset;
          }
        }

        boost::asio::async_read(stream_in, input_buffer,
            boost::asio::transfer_at_least(1), processor);
      };

  boost::asio::async_read(
      stream_in, input_buffer, boost::asio::transfer_at_least(1), processor);

  ioc_.run();
}

/* in worker thread */
void Client::Impl::SendMessageToMainThread(
    std::shared_ptr<message::IMessage> message) {
  message::Action action = message->GetAction();

  if (write(event_pipe_[1], &action, sizeof(message::Action)) !=
      sizeof(message::Action)) {
    std::cerr << "write miss";
  };

  uint32_t size = message->GetSize();
  if (write(event_pipe_[1], &size, sizeof(uint32_t)) != sizeof(uint32_t)) {
    std::cerr << "write miss";
  };

  message->WriteTo(event_pipe_[1]);
  fsync(event_pipe_[1]);
}

/* in worker thread */
void Client::Impl::OnNewEvent(std::string json) {
  auto message = MessageParser::ParseEvent(json);

  SendMessageToMainThread(message);
}

/* in worker thread */
void Client::Impl::OnMessagingFail(std::string reason) {
  auto error = std::make_shared<message::Error>(reason);

  SendMessageToMainThread(error);
}

}  // namespace zigen_playground
