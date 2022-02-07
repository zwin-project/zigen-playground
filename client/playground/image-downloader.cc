#include "image-downloader.h"

#include <curl/curl.h>
#include <fcntl.h>
#include <unistd.h>

#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <iostream>
#include <thread>

static size_t WriteData(
    char *buffer, size_t size, size_t nmemb, std::string *buf) {
  size_t buffer_size = size * nmemb;
  buf->insert(buf->end(), buffer, buffer + buffer_size);
  return buffer_size;
}

namespace zigen_playground {

ImageDownloader::ImageDownloader(std::string url,
    std::function<void(uint32_t width, uint32_t height, uint32_t pixel_size,
        std::function<void(void *data)> writer)>
        callback)
    : success_(false), url_(url), callback_(callback) {
  int p[2];

  if (pipe2(p, O_CLOEXEC | O_NONBLOCK) == -1) throw "failed to create pipe";

  fd_ = p[0];
  thread_fd_ = p[1];
  op_ = EPOLL_CTL_ADD;

  epoll_event_.events = EPOLLIN;

  thread_ = std::thread([&] {
    this->InThread();
    close(this->thread_fd_);
  });
}

ImageDownloader::~ImageDownloader() { thread_.join(); }

bool ImageDownloader::Done([[maybe_unused]] struct epoll_event *ev) {
  std::cerr << "downloaded " << url_ << std::endl;
  std::cerr << buf_.size() << "bytes" << std::endl;

  std::istringstream is(buf_, std::istringstream::binary);

  boost::gil::bgra8_image_t img;
  try {
    boost::gil::read_and_convert_image(is, img, boost::gil::jpeg_tag());
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return true;
  }

  auto view = boost::gil::view(img);

  uint32_t w = img.width();
  uint32_t h = img.height();
  uint32_t pixel_size = boost::gil::num_channels<boost::gil::bgra8_image_t>();

  callback_(w, h, pixel_size, [&](void *data) {
    boost::gil::copy_pixels(
        view, boost::gil::interleaved_view(w, h,
                  reinterpret_cast<boost::gil::bgra8_pixel_t *>(data),
                  w * pixel_size));
  });

  return true;
}

void ImageDownloader::InThread() {
  CURL *curl = curl_easy_init();
  CURLcode res;
  if (curl == NULL) return;

  curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf_);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

  res = curl_easy_perform(curl);

  if (res != CURLE_OK) return;

  curl_easy_cleanup(curl);
}

}  // namespace zigen_playground
