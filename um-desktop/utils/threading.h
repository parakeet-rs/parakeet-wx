#pragma once

#include <boost/asio/io_service.hpp>

namespace umd {

void init_thread_workers();
extern boost::asio::io_service io_service;

}  // namespace umd
