#ifndef ZIGEN_PLAYGROUND_MESSAGES_SYNC_H
#define ZIGEN_PLAYGROUND_MESSAGES_SYNC_H

#include <vector>

#include "message.h"
#include "model.h"

namespace zigen_playground {
namespace message {

class SyncEvent : public IMessage {
 public:
  SyncEvent();
  SyncEvent(std::string json);

  virtual bool WriteTo(int fd) override final;

  virtual bool Load(void *data) override final;

  virtual uint32_t GetSize() override final;

  virtual message::Action GetAction() override final;

  std::vector<std::shared_ptr<model::Resource>> GetResources();

 private:
  std::string json_;
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_SYNC_H
