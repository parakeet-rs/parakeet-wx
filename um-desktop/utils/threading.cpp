#include "threading.h"

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

constexpr int kThreadCount = 4;

namespace asio = boost::asio;

namespace umd {

asio::io_service io_service;
asio::io_service::work worker(io_service);
boost::thread_group threads;

void io_service_start() {
  for (std::size_t i = 0; i < kThreadCount; ++i) {
    threads.create_thread(boost::bind(&asio::io_service::run, &io_service));
  }
}

void io_service_stop() {
  io_service.stop();
  threads.join_all();
}

}  // namespace umd
