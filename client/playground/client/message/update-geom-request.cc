#include "update-geom-request.h"

#include <unistd.h>

namespace zigen_playground {
namespace message {

static std::string Vec3ToString(glm::vec3 v) {
  return "[" + std::to_string(v[0]) + "," + std::to_string(v[1]) + "," +
         std::to_string(v[2]) + "]";
}

UpdateGeomRequest::UpdateGeomRequest() {}

UpdateGeomRequest::UpdateGeomRequest(std::shared_ptr<model::Sphere> sphere) {
  // clang-format off
  json_ =
      "{"
      "  \"action\": \"update-geom\","
      "  \"data\": {"
      "    \"id\": " + std::to_string(sphere->id) + ","
      "    \"type\": \"sphere\","
      "    \"position\": " + Vec3ToString(sphere->position) + ","
      "    \"r\": " + std::to_string(sphere->r) + ""
      "  }"
      "}";
  // clang-format on
}

bool UpdateGeomRequest::WriteTo(int fd) {
  uint32_t size = json_.size();
  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool UpdateGeomRequest::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t UpdateGeomRequest::GetSize() {
  return sizeof(uint32_t) + json_.size();
}

message::Action UpdateGeomRequest::GetAction() {
  return Action::kUpdateGeomRequest;
}

std::string UpdateGeomRequest::ToJson() { return json_; }

}  // namespace message
}  // namespace zigen_playground
