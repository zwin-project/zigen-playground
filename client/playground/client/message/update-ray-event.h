#ifndef ZIGEN_PLAYGROUND_MESSAGES_UPDATE_RAY_EVENT_H
#define ZIGEN_PLAYGROUND_MESSAGES_UPDATE_RAY_EVENT_H

#include "message.h"
#include "model.h"

namespace zigen_playground {
namespace message {

class UpdateRayEvent : public IMessage {
 public:
  UpdateRayEvent();
  UpdateRayEvent(std::string json);

  virtual bool WriteTo(int fd) override final;

  virtual bool Load(void *data) override final;

  virtual uint32_t GetSize() override final;

  virtual message::Action GetAction() override final;

  std::string GetType();

  std::shared_ptr<model::Ray> GetRay();

 private:
  std::string json_;
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_UPDATE_RAY_EVENT_H
