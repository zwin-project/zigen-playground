#ifndef ZIGEN_PLAYGROUND_MESSAGES_NEW_TEXTURE_REQUEST_H
#define ZIGEN_PLAYGROUND_MESSAGES_NEW_TEXTURE_REQUEST_H

#include <string>

#include "message.h"

namespace zigen_playground {
namespace message {

class NewTextureRequest : public IRequestMessage {
 public:
  NewTextureRequest();

  NewTextureRequest(uint64_t resource_id, std::string url);

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

#endif  //  ZIGEN_PLAYGROUND_MESSAGES_NEW_TEXTURE_REQUEST_H
