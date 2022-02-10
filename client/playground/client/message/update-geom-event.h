#ifndef ZIGEN_PLAYGROUND_MESSAGES_UPDATE_GEOM_EVENT_H
#define ZIGEN_PLAYGROUND_MESSAGES_UPDATE_GEOM_EVENT_H

#include "message.h"
#include "model.h"

namespace zigen_playground {
namespace message {

class UpdateGeomEvent : public IMessage {
 public:
  UpdateGeomEvent();
  UpdateGeomEvent(std::string json);

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

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_UPDATE_GEOM_EVENT_H
