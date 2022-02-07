#include "sync-event.h"

#include <unistd.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace zigen_playground {
namespace message {

glm::vec3 ToVec3(boost::property_tree::ptree const &pt,
    boost::property_tree::ptree::key_type const &key) {
  glm::vec3 v;
  int i = 0;
  for (auto &item : pt.get_child(key)) {
    v[i++] = item.second.get_value<float>();
    if (i >= 3) break;
  }

  return v;
}

SyncEvent::SyncEvent() {}

SyncEvent::SyncEvent(std::string json) : json_(json) {}

bool SyncEvent::WriteTo(int fd) {
  uint32_t size = json_.size();

  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool SyncEvent::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t SyncEvent::GetSize() { return sizeof(uint32_t) + json_.size(); }

message::Action SyncEvent::GetAction() { return message::Action::kSyncEvent; }

std::vector<std::shared_ptr<model::Resource>> SyncEvent::GetResources() {
  std::stringstream ss;
  std::vector<std::shared_ptr<model::Resource>> resources;
  boost::property_tree::ptree property_tree;

  ss << json_;
  boost::property_tree::read_json(ss, property_tree);

  auto data = property_tree.get_child("data");
  for (auto child : data) {
    const boost::property_tree::ptree resource = child.second;

    auto type = resource.get<std::string>("type");

    if (type == "cuboid") {
      glm::vec3 position = ToVec3(resource, "position");
      glm::vec3 half_size = ToVec3(resource, "half_size");

      auto cuboid = std::make_shared<model::Cuboid>(position, half_size);

      resources.push_back(cuboid);
    } else if (type == "sphere") {
      glm::vec3 position = ToVec3(resource, "position");
      float r = resource.get<float>("r");
      uint32_t resolution = resource.get<uint32_t>("resolution");

      auto sphere = std::make_shared<model::Sphere>(position, r, resolution);

      resources.push_back(sphere);
    }
  }

  return resources;
}

}  // namespace message
}  // namespace zigen_playground
