#include "client.h"

#include <zigen-playground.h>

#include "client-impl.h"

namespace zigen_playground {

std::shared_ptr<Client> Client::Create(std::shared_ptr<zukou::Application> app,
    std::string remote_host, std::string remote_port, std::string remote_path) {
  return std::shared_ptr<Client>(
      new Client(app, remote_host, remote_port, remote_path));
}

Client::Client(std::shared_ptr<zukou::Application> app, std::string remote_host,
    std::string remote_port, std::string remote_path)
    : pimpl_(
          std::make_shared<Impl>(app, remote_host, remote_port, remote_path)) {}

bool Client::Connect() { return pimpl_->Connect(); }

void Client::SyncRequest() { pimpl_->SyncRequest(); }

void Client::NewResourceRequest(std::string resource_type, glm::vec3 position) {
  pimpl_->NewResourceRequest(resource_type, position);
}

void Client::ConnectNoopEventSignal(std::function<void()> observer) {
  pimpl_->noop_event_signal.connect(observer);
}

void Client::ConnectSyncEventSignal(
    std::function<void(std::vector<std::shared_ptr<model::Resource>>)>
        observer) {
  pimpl_->sync_event_signal.connect(observer);
}

void Client::ConnectNewResourceEventSignal(
    std::function<void(std::shared_ptr<model::Resource>)> observer) {
  pimpl_->new_resource_event_signal.connect(observer);
}

void Client::ConnectErrorSignal(std::function<void(std::string)> observer) {
  pimpl_->error_signal.connect(observer);
}

}  // namespace zigen_playground
