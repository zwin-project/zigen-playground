#ifndef ZIGEN_PLAYGROUND_DND_MESSAGE_PARSER_H
#define ZIGEN_PLAYGROUND_DND_MESSAGE_PARSER_H

#include <memory>
#include <string>

#include "dnd-message.h"

namespace zigen_playground {

class DndMessageParser {
 public:
  std::shared_ptr<IDndMessage> Parse(std::string);
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_DND_MESSAGE_PARSER_H
