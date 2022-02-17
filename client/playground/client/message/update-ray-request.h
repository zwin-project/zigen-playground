#ifndef ZIGEN_PLAYGROUND_MESSAGES_UPDATE_RAY_REQUEST_H
#define ZIGEN_PLAYGROUND_MESSAGES_UPDATE_RAY_REQUEST_H

#include "message.h"
#include "model.h"

namespace zigen_playground {
namespace message {

class UpdateRayRequest : public IRequestMessage {
 public:
  UpdateRayRequest();

  UpdateRayRequest(glm::vec3 origin, glm::vec3 target);

  virtual bool WriteTo(int fd) override final;

  virtual bool Load(void *data) override final;

  virtual uint32_t GetSize() override final;

  virtual message::Action GetAction() override final;

  virtual std::string ToJson() override final;

 private:
  std::string json_;
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_UPDATE_RAY_REQUEST_H
