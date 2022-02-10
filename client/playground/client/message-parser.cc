#include "message-parser.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <sstream>
#include <string>

#include "message/error.h"
#include "message/new-resource-event.h"
#include "message/new-texture-event.h"
#include "message/noop-event.h"
#include "message/sync-event.h"
#include "message/update-geom-event.h"

namespace zigen_playground {

std::shared_ptr<message::IMessage> MessageParser::ParseEvent(std::string json) {
  std::stringstream ss;
  ss << json;
  boost::property_tree::ptree property_tree;
  boost::property_tree::read_json(ss, property_tree);
  auto action = property_tree.get_optional<std::string>("action");

  if (!action) return std::make_shared<message::Error>("action not found");

  if (*action == "noop") return std::make_shared<message::NoopEvent>();

  if (*action == "sync") return std::make_shared<message::SyncEvent>(json);

  if (*action == "new-resource")
    return std::make_shared<message::NewResourceEvent>(json);

  if (*action == "new-texture")
    return std::make_shared<message::NewTextureEvent>(json);

  if (*action == "update-geom") return std::make_shared<message::UpdateGeomEvent>(json);

  return std::make_shared<message::Error>("invalid action: " + *action);
}

}  // namespace zigen_playground
