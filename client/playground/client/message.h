#ifndef ZIGEN_PLAYGROUND_MESSAGE_H
#define ZIGEN_PLAYGROUND_MESSAGE_H

#include <iostream>
#include <string>

namespace zigen_playground {

namespace message {

enum class Action : uint32_t {
  kUnset,
  kNewResourceEvent,
  kNewResourceRequest,
  kNewTextureEvent,
  kNewTextureRequest,
  kNoopEvent,
  kSyncEvent,
  kSyncRequest,
  kUpdateGeomRequest,
  kUpdateGeomEvent,
  kUpdateRayRequest,
  kUpdateRayEvent,
  kError,
};

class IMessage {
 public:
  virtual bool WriteTo(int fd) = 0;
  virtual bool Load(void* data) = 0;
  virtual uint32_t GetSize() = 0;
  virtual message::Action GetAction() = 0;
  virtual ~IMessage(){};
};

class IRequestMessage : public IMessage {
 public:
  virtual std::string ToJson() = 0;
  virtual ~IRequestMessage() {}
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGE_H
