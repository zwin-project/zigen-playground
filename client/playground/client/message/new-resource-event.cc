#include "new-resource-event.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "helper.h"

namespace zigen_playground {
namespace message {

NewResourceEvent::NewResourceEvent() {}

NewResourceEvent::NewResourceEvent(std::string json) : json_(json) {}

bool NewResourceEvent::WriteTo(int fd) {
  uint32_t size = json_.size();

  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool NewResourceEvent::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t NewResourceEvent::GetSize() { return sizeof(uint32_t) + json_.size(); }

message::Action NewResourceEvent::GetAction() {
  return message::Action::kNewResourceEvent;
}

std::shared_ptr<model::Resource> NewResourceEvent::GetResource() {
  std::stringstream ss;
  boost::property_tree::ptree property_tree;

  ss << json_;
  boost::property_tree::read_json(ss, property_tree);

  auto type = property_tree.get<std::string>("data.type");

  if (type == "cuboid") {
    glm::vec3 position = ToVec3(property_tree, "data.position");
    glm::vec3 half_size = ToVec3(property_tree, "data.half_size");

    return std::make_shared<model::Cuboid>(position, half_size);
  } else if (type == "sphere") {
    glm::vec3 position = ToVec3(property_tree, "data.position");
    float r = property_tree.get<float>("data.r");
    uint32_t resolution = property_tree.get<uint32_t>("data.resolution");
    std::string texture = property_tree.get<std::string>("data.texture");

    return std::make_shared<model::Sphere>(position, r, resolution, texture);
  }

  return std::shared_ptr<model::Resource>();
}

}  // namespace message
}  // namespace zigen_playground
