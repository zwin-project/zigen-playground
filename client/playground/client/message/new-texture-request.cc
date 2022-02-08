#include "new-texture-request.h"

#include <unistd.h>

namespace zigen_playground {
namespace message {

NewTextureRequest::NewTextureRequest() {}

NewTextureRequest::NewTextureRequest(uint64_t resource_id, std::string url) {
  // clang-format off
  json_ =
      "{"
      "  \"action\": \"new-texture\","
      "  \"data\": {"
      "    \"id\": " + std::to_string(resource_id) + ","
      "    \"url\": \"" + url + "\""
      "  }"
      "}";
  // clang-format on
}

bool NewTextureRequest::WriteTo(int fd) {
  uint32_t size = json_.size();
  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool NewTextureRequest::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t NewTextureRequest::GetSize() {
  return sizeof(uint32_t) + json_.size();
}

message::Action NewTextureRequest::GetAction() {
  return Action::kNewTextureRequest;
}

std::string NewTextureRequest::ToJson() { return json_; }

}  // namespace message
}  // namespace zigen_playground
