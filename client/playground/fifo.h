#ifndef ZIGEN_PLAYGROUND_FIFO_H
#define ZIGEN_PLAYGROUND_FIFO_H

#include <cstring>
#include <vector>

namespace zigen_playground {

template <typename T>
class Fifo {
 public:
  void Push(T* data, std::size_t count) {
    std::vector<T> v(data, data + count);
    data_.insert(data_.end(), v.begin(), v.end());
  };

  /** @returns the data size in bytes */
  std::size_t Size() { return data_.size() * sizeof(T); };

  void Pop(T* data, std::size_t count) {
    memcpy(data, data_.data(), count * sizeof(T));
    data_ = std::vector<T>(data_.begin() + count, data_.end());
  };

 private:
  std::vector<T> data_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_FIFO_H
