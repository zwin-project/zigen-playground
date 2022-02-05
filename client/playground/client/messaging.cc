#include "messaging.h"

#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

namespace zigen_playground {

std::shared_ptr<Messaging> Messaging::Create(boost::asio::io_context& ioc) {
  return std::shared_ptr<Messaging>(new Messaging(ioc));
}

Messaging::Messaging(boost::asio::io_context& ioc)
    : resolver_(boost::asio::make_strand(ioc)),
      ws_(boost::asio::make_strand(ioc)) {}

bool Messaging::Run(std::string host, std::string port, std::string path) {
  host_ = host;
  port_ = port;
  path_ = path;

  auto results = resolver_.resolve(host, port);

  auto endpoint = beast::get_lowest_layer(ws_).connect(results);

  ws_.set_option(
      websocket::stream_base::timeout::suggested(beast::role_type::client));

  ws_.set_option(
      websocket::stream_base::decorator([](websocket::request_type& req) {
        req.set(http::field::user_agent,
            std::string(BOOST_BEAST_VERSION_STRING) +
                " websocket-client-async-zigen-playground");
      }));

  host_ += ":" + std::to_string(endpoint.port());

  ws_.handshake(host_, path_);

  ws_.async_read(buffer_,
      beast::bind_front_handler(&Messaging::OnRead, shared_from_this()));

  return true;
}

void Messaging::OnRead(
    boost::beast::error_code error_code, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (error_code) return Fail(error_code, "read");

  OnNewEvent(boost::beast::buffers_to_string(buffer_.data()));

  buffer_.clear();

  ws_.async_read(buffer_,
      beast::bind_front_handler(&Messaging::OnRead, shared_from_this()));
}

void Messaging::OnWrite(
    boost::beast::error_code error_code, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (error_code) Fail(error_code, "write");
}

void Messaging::Fail(
    boost::beast::error_code error_code, std::string error_point) {
  OnFail(error_point + ": " + error_code.message());
}

void Messaging::Write(std::string message) {
  ws_.async_write(boost::asio::buffer(message, message.size()),
      beast::bind_front_handler(&Messaging::OnWrite, shared_from_this()));
}

}  // namespace zigen_playground
