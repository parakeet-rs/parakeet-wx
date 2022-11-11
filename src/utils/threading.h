#pragma once

#include <boost/asio/io_service.hpp>
#include <cstddef>

namespace umd {

void io_service_start(size_t thread_count);
void io_service_stop();
extern boost::asio::io_service io_service;

}  // namespace umd
