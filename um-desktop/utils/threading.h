#pragma once

#include <boost/asio/io_service.hpp>

namespace umd {

void io_service_start();
void io_service_stop();
extern boost::asio::io_service io_service;

}  // namespace umd
