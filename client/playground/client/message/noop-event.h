#ifndef ZIGEN_PLAYGROUND_MESSAGES_NOOP_H
#define ZIGEN_PLAYGROUND_MESSAGES_NOOP_H

#include "message.h"

namespace zigen_playground {
namespace message {

class NoopEvent : public IMessage {
 public:
  virtual bool WriteTo(int fd) override final;

  virtual bool Load(void *data) override final;

  virtual uint32_t GetSize() override final;

  virtual message::Action GetAction() override final;
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_NOOP_H
