#include "noop-event.h"

#include <unistd.h>

namespace zigen_playground {
namespace message {

bool NoopEvent::WriteTo(int fd) {
  return write(fd, "noop message", sizeof("noop message")) ==
         sizeof("noop message");
}

bool NoopEvent::Load(void *data) {
  (void)data;
  return true;
}

uint32_t NoopEvent::GetSize() { return sizeof("noop message"); }

message::Action NoopEvent::GetAction() { return message::Action::kNoopEvent; }

}  // namespace message
}  // namespace zigen_playground
