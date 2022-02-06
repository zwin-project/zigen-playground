#ifndef ZIGEN_PLAYGROUND_CLIENT_H
#define ZIGEN_PLAYGROUND_CLIENT_H

#include <zukou/zukou.h>

#include <functional>
#include <memory>
#include <vector>

#include "model.h"

namespace zigen_playground {

class ClientEventPollEvent;

class Client : public std::enable_shared_from_this<Client> {
  friend ClientEventPollEvent;

 public:
  static std::shared_ptr<Client> Create(std::shared_ptr<zukou::Application> app,
      std::string remote_host, std::string remote_port,
      std::string remote_path);

 public:
  bool Connect();

  void ConnectNoopEventSignal(std::function<void()> observer);
  void ConnectSyncEventSignal(
      std::function<void(std::vector<std::shared_ptr<model::Resource>>)>
          observer);
  void ConnectErrorSignal(std::function<void(std::string)> observer);

  void SyncRequest();
  void NewResourceRequest(std::string resource_type, glm::vec3 position);

 private:
  Client(std::shared_ptr<zukou::Application> app, std::string remote_host,
      std::string remote_port, std::string remote_path);

 private:
  class Impl;
  std::shared_ptr<Impl> pimpl_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_CLIENT_H
