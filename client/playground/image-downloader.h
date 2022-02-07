#ifndef ZIGEN_PLAYGROUND_IMAGE_DOWNLOADER_H
#define ZIGEN_PLAYGROUND_IMAGE_DOWNLOADER_H

#include <zukou/zukou.h>

#include <functional>
#include <thread>

namespace zigen_playground {

class ImageDownloader : public zukou::PollEvent {
  enum class ImageType {
    kUnkown,
    kJpeg,
    kPng,
  };

 public:
  ImageDownloader(std::string url,
      std::function<void(uint32_t width, uint32_t height, uint32_t pixel_size,
          std::function<void(void* data)> writer)>
          callback);
  ~ImageDownloader();

  virtual bool Done(struct epoll_event* ev) override final;

 private:
  void InThread();
  void GetImageType();

  std::thread thread_;
  bool success_;
  std::string buf_;
  int thread_fd_;
  const std::string url_;
  std::function<void(uint32_t width, uint32_t height, uint32_t pixel_size,
      std::function<void(void* data)> writer)>
      callback_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_IMAGE_DOWNLOADER_H
