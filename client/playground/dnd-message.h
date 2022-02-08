#ifndef ZIGEN_PLAYGROUND_DND_MESSAGE_H
#define ZIGEN_PLAYGROUND_DND_MESSAGE_H

#include <string>

namespace zigen_playground {

enum class DndMessageType {
  kError,
  kNewResource,
  kNewTexture,
};

class IDndMessage {
 public:
  virtual enum DndMessageType type() = 0;
  virtual ~IDndMessage() {}
};

class DndErrorMessage : public IDndMessage {
 public:
  virtual enum DndMessageType type() { return DndMessageType::kError; }
};

class DndNewResourceMessage : public IDndMessage {
 public:
  DndNewResourceMessage(std::string resource_type)
      : resource_type_(resource_type){};

  virtual enum DndMessageType type() { return DndMessageType::kNewResource; }

  std::string resource_type() { return resource_type_; };

 private:
  std::string resource_type_;
};

class DndNewTextureMessage : public IDndMessage {
 public:
  DndNewTextureMessage(std::string url) : url_(url) {}

  virtual enum DndMessageType type() { return DndMessageType::kNewTexture; }

  std::string url() { return url_; }

 private:
  std::string url_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_DND_MESSAGE_H
