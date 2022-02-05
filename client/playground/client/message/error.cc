#include "error.h"

#include <unistd.h>

#include <memory>

namespace zigen_playground {
namespace message {

Error::Error() {}

Error::Error(std::string reason) { reason_ = reason; }

bool Error::WriteTo(int fd) {
  uint32_t size = reason_.size();
  if (write(fd, &size, sizeof(uint32_t)) != sizeof(uint32_t)) return false;

  return write(fd, reason_.c_str(), reason_.size()) ==
         static_cast<ssize_t>(reason_.size());
}

bool Error::Load(void *data) {
  uint32_t size = reinterpret_cast<uint32_t *>(data)[0];
  char *str = reinterpret_cast<char *>(data) + 4;
  reason_ = std::string(str, size);
  return true;
}

uint32_t Error::GetSize() { return sizeof(uint32_t) + reason_.size(); }

message::Action Error::GetAction() { return message::Action::kError; }

}  // namespace message
}  // namespace zigen_playground
