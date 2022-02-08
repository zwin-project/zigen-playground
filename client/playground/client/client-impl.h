#ifndef ZIGEN_PLAYGROUND_CLIENT_IMPL_H
#define ZIGEN_PLAYGROUND_CLIENT_IMPL_H

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/bind.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <memory>

#include "client.h"
#include "fifo.h"
#include "message.h"

namespace zigen_playground {

class Client::Impl : public std::enable_shared_from_this<Impl> {
  friend ClientEventPollEvent;

 public:
  Impl(std::shared_ptr<zukou::Application> app, std::string remote_host,
      std::string remote_port, std::string remote_path);
  ~Impl();

  bool Connect();
  void SyncRequest();
  void NewResourceRequest(std::string resource_type, glm::vec3 position);
  void NewTextureRequest(uint64_t resource_id, std::string url);

  boost::signals2::signal<void()> noop_event_signal;
  boost::signals2::signal<void(std::vector<std::shared_ptr<model::Resource>>)>
      sync_event_signal;
  boost::signals2::signal<void(std::shared_ptr<model::Resource>)>
      new_resource_event_signal;
  boost::signals2::signal<void(uint64_t resource_id, std::string texture_url)> new_texture_event_signal;
  boost::signals2::signal<void(std::string)> error_signal;

 private:
  void OnEvent();
  void NotifyRequest(std::shared_ptr<message::IRequestMessage> request);

  /* functions in worker thread */
  void Run();
  void SendMessageToMainThread(std::shared_ptr<message::IMessage> message);
  void OnNewEvent(std::string json);
  void OnMessagingFail(std::string reason);
  /* functions in worker thread (end) */

 private:
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::PollEvent> client_event_poll_event_;
  const std::string remote_host_;
  const std::string remote_port_;
  const std::string remote_path_;

  boost::thread client_thread_;
  boost::asio::io_context ioc_;
  int event_pipe_[2];    // 1.worker --> 0.main
  int request_pipe_[2];  // 1.main --> 0.worker

  message::Action handling_event_;
  uint32_t event_message_size_;
  Fifo<char> event_message_buffer_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_CLIENT_IMPL_H
