#ifndef ZIGEN_PLAYGROUND_MESSAGES_NEW_TEXTURE_EVENT_H
#define ZIGEN_PLAYGROUND_MESSAGES_NEW_TEXTURE_EVENT_H

#include "message.h"
#include "model.h"

namespace zigen_playground {
namespace message {

class NewTextureEvent : public IMessage {
 public:
  NewTextureEvent();
  NewTextureEvent(std::string json);

  virtual bool WriteTo(int fd) override final;

  virtual bool Load(void *data) override final;

  virtual uint32_t GetSize() override final;

  virtual message::Action GetAction() override final;

  uint64_t GetResourceId();

  std::string GetTextureUrl();

 private:
  std::string json_;
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_NEW_TEXTURE_EVENT_H
