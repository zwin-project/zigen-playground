#ifndef ZIGEN_PLAYGROUND_MESSAGING_H
#define ZIGEN_PLAYGROUND_MESSAGING_H

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/bind.hpp>
#include <boost/signals2/signal.hpp>
#include <memory>
#include <string>

namespace zigen_playground {

class Messaging : public std::enable_shared_from_this<Messaging> {
 public:
  static std::shared_ptr<Messaging> Create(boost::asio::io_context& ioc);

 public:
  bool Run(std::string host, std::string port, std::string path);
  void Write(std::string message);

  boost::signals2::signal<void(std::string json)> OnNewEvent;
  boost::signals2::signal<void(std::string error_message)> OnFail;

 private:
  Messaging(boost::asio::io_context& ioc);

  void OnRead(
      boost::beast::error_code error_code, std::size_t bytes_transferred);

  void Fail(boost::beast::error_code error_code, std::string error_point);

 private:
  boost::asio::ip::tcp::resolver resolver_;
  boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
  boost::beast::flat_buffer buffer_;
  std::string host_;
  std::string port_;
  std::string path_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGING_H
