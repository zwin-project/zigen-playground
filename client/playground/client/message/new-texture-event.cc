#include "new-texture-event.h"

#include <unistd.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace zigen_playground {
namespace message {

NewTextureEvent::NewTextureEvent() {}

NewTextureEvent::NewTextureEvent(std::string json) : json_(json) {}

bool NewTextureEvent::WriteTo(int fd) {
  uint32_t size = json_.size();

  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, json_.c_str(), json_.size()) ==
         static_cast<ssize_t>(json_.size());
}

bool NewTextureEvent::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  json_ = std::string(str, size);
  return true;
}

uint32_t NewTextureEvent::GetSize() { return sizeof(uint32_t) + json_.size(); }

message::Action NewTextureEvent::GetAction() {
  return message::Action::kNewTextureEvent;
}

uint64_t NewTextureEvent::GetResourceId() {
  std::stringstream ss;
  boost::property_tree::ptree property_tree;

  ss << json_;
  boost::property_tree::read_json(ss, property_tree);

  return property_tree.get<uint64_t>("data.id");
}

std::string NewTextureEvent::GetTextureUrl() {
  std::stringstream ss;
  boost::property_tree::ptree property_tree;

  ss << json_;
  boost::property_tree::read_json(ss, property_tree);

  return property_tree.get<std::string>("data.url");
}

}  // namespace message
}  // namespace zigen_playground
