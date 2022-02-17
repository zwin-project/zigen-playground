#include "update-ray-event.h"

#include <unistd.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <memory>

#include "helper.h"

namespace zigen_playground {
namespace message {

UpdateRayEvent::UpdateRayEvent() {}

UpdateRayEvent::UpdateRayEvent(std::string json) : json_(json) {}

bool UpdateRayEvent::WriteTo(int fd) {
  uint32_t size = json_.size();

  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool UpdateRayEvent::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t UpdateRayEvent::GetSize() { return sizeof(uint32_t) + json_.size(); }

message::Action UpdateRayEvent::GetAction() {
  return message::Action::kUpdateRayEvent;
}

std::string UpdateRayEvent::GetType() {
  std::stringstream ss;
  boost::property_tree::ptree property_tree;

  ss << json_;
  boost::property_tree::read_json(ss, property_tree);

  return property_tree.get<std::string>("data.type");
}

std::shared_ptr<model::Ray> UpdateRayEvent::GetRay() {
  std::stringstream ss;
  boost::property_tree::ptree property_tree;

  ss << json_;
  boost::property_tree::read_json(ss, property_tree);

  uint64_t client_id = property_tree.get<uint64_t>("data.clientId");

  glm::vec3 origin = ToVec3(property_tree, "data.origin");
  glm::vec3 target = ToVec3(property_tree, "data.target");

  return std::make_shared<model::Ray>(client_id, origin, target);
}

}  // namespace message

}  // namespace zigen_playground
