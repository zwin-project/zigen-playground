#ifndef ZIGEN_PLAYGROUND_ERROR_H
#define ZIGEN_PLAYGROUND_ERROR_H

#include "message.h"

namespace zigen_playground {
namespace message {

class Error : public IMessage {
 public:
  Error();
  Error(std::string reason);

  virtual bool WriteTo(int fd) override final;

  virtual bool Load(void *data) override final;

  virtual uint32_t GetSize() override final;

  virtual message::Action GetAction() override final;

  inline std::string reason() { return reason_; };

 private:
  std::string reason_;
};

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_ERROR_H
