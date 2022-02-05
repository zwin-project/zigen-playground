#include "sync-request.h"

#include <unistd.h>

namespace zigen_playground {
namespace message {

SyncRequest::SyncRequest() { json_ = "{\"action\": \"sync\"}"; }

bool SyncRequest::WriteTo(int fd) {
  uint32_t size = json_.size();

  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool SyncRequest::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t SyncRequest::GetSize() { return sizeof(uint32_t) + json_.size(); }

message::Action SyncRequest::GetAction() {
  return message::Action::kSyncRequest;
}

std::string SyncRequest::ToJson() { return json_; }

}  // namespace message
}  // namespace zigen_playground
