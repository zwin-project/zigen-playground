#include "update-ray-request.h"

#include <unistd.h>

namespace zigen_playground {
namespace message {

static std::string Vec3ToString(glm::vec3 v) {
  return "[" + std::to_string(v[0]) + "," + std::to_string(v[1]) + "," +
         std::to_string(v[2]) + "]";
}

UpdateRayRequest::UpdateRayRequest() {}

UpdateRayRequest::UpdateRayRequest(glm::vec3 origin, glm::vec3 target) {
  // clang-format off
  json_ =
      "{"
      "  \"action\": \"update-ray\","
      "  \"data\": {"
      "    \"origin\": " + Vec3ToString(origin) + ","
      "    \"target\": " + Vec3ToString(target) + ""
      "  }"
      "}";
  // clang-format on
}

bool UpdateRayRequest::WriteTo(int fd) {
  uint32_t size = json_.size();
  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool UpdateRayRequest::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t UpdateRayRequest::GetSize() { return sizeof(uint32_t) + json_.size(); }

message::Action UpdateRayRequest::GetAction() {
  return Action::kUpdateRayRequest;
}

std::string UpdateRayRequest::ToJson() { return json_; }

}  // namespace message
}  // namespace zigen_playground
