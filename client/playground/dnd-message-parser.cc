#include "dnd-message-parser.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <sstream>

namespace zigen_playground {

static std::shared_ptr<IDndMessage> ParseError(
    [[maybe_unused]] boost::property_tree::ptree property_tree) {
  return std::make_shared<DndErrorMessage>();
}

static std::shared_ptr<IDndMessage> ParseNewResource(
    boost::property_tree::ptree property_tree) {
  auto resource_type = property_tree.get_optional<std::string>("data.type");

  if (!resource_type) return ParseError(property_tree);

  return std::make_shared<DndNewResourceMessage>(*resource_type);
}

std::shared_ptr<IDndMessage> DndMessageParser::Parse(std::string text) {
  std::stringstream ss;
  ss << text;
  boost::property_tree::ptree property_tree;
  boost::property_tree::read_json(ss, property_tree);
  auto action = property_tree.get_optional<std::string>("action");

  if (!action) return ParseError(property_tree);

  if (*action == "new-resource") return ParseNewResource(property_tree);

  return ParseError(property_tree);
}

}  // namespace zigen_playground
