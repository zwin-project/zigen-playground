#ifndef ZIGEN_PLAYGROUND_MESSAGES_NEW_RESOURCE_EVENT_H
#define ZIGEN_PLAYGROUND_MESSAGES_NEW_RESOURCE_EVENT_H

#include <vector>

#include "message.h"
#include "model.h"

namespace zigen_playground {
namespace message {

class NewResourceEvent : public IMessage {
 public:
  NewResourceEvent();
  NewResourceEvent(std::string json);

  virtual bool WriteTo(int fd) override final;

  virtual bool Load(void *data) override final;

  virtual uint32_t GetSize() override final;

  virtual message::Action GetAction() override final;

  std::shared_ptr<model::Resource> GetResource();

 private:
  std::string json_;
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_NEW_RESOURCE_EVENT_H
