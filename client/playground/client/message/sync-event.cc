#include "sync-event.h"

#include <unistd.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "helper.h"

namespace zigen_playground {
namespace message {

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
      uint64_t id = resource.get<uint64_t>("id");
      glm::vec3 position = ToVec3(resource, "position");
      glm::vec3 half_size = ToVec3(resource, "half_size");

      auto cuboid = std::make_shared<model::Cuboid>(id, position, half_size);

      resources.push_back(cuboid);
    } else if (type == "sphere") {
      uint64_t id = resource.get<uint64_t>("id");
      glm::vec3 position = ToVec3(resource, "position");
      float r = resource.get<float>("r");
      uint32_t resolution = resource.get<uint32_t>("resolution");
      std::string texture = resource.get<std::string>("texture");

      auto sphere =
          std::make_shared<model::Sphere>(id, position, r, resolution, texture);

      resources.push_back(sphere);
    }
  }

  return resources;
}

}  // namespace message
}  // namespace zigen_playground
