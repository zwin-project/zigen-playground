#include "new-resource-request.h"

#include <unistd.h>

namespace zigen_playground {
namespace message {

static std::string Vec3ToString(glm::vec3 v) {
  return "[" + std::to_string(v[0]) + "," + std::to_string(v[1]) + "," +
         std::to_string(v[2]) + "]";
}

NewResourceRequest::NewResourceRequest() {}

NewResourceRequest::NewResourceRequest(
    std::string resource_type, glm::vec3 position) {
  // clang-format off
  json_ =
      "{"
      "  \"action\": \"new-resource\","
      "  \"data\": {"
      "    \"type\": \"" + resource_type + "\","
      "    \"position\": " + Vec3ToString(position) + ""
      "  }"
      "}";
  // clang-format on
}

bool NewResourceRequest::WriteTo(int fd) {
  uint32_t size = json_.size();
  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool NewResourceRequest::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t NewResourceRequest::GetSize() {
  return sizeof(uint32_t) + json_.size();
}

message::Action NewResourceRequest::GetAction() {
  return Action::kNewResourceRequest;
}

std::string NewResourceRequest::ToJson() { return json_; }

}  // namespace message
}  // namespace zigen_playground
