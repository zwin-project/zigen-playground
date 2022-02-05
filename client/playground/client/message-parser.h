#ifndef ZIGEN_PLAYGROUND_MESSAGE_PARSER_H
#define ZIGEN_PLAYGROUND_MESSAGE_PARSER_H

#include <memory>
#include <string>

#include "message.h"

namespace zigen_playground {

class MessageParser {
 public:
  static std::shared_ptr<message::IMessage> ParseEvent(std::string json);
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGE_PARSER_H
