#include "threading.h"

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

#include <algorithm>
#include <cstdint>

namespace asio = boost::asio;

namespace parakeet_wx
{

asio::io_service io_service;
asio::io_service::work worker(io_service);
boost::thread_group threads;

void io_service_start(size_t thread_count)
{
    thread_count = std::max(thread_count, size_t{1});
    for (std::size_t i = 0; i < thread_count; ++i)
    {
        threads.create_thread(boost::bind(&asio::io_service::run, &io_service));
    }
}

void io_service_stop()
{
    io_service.stop();
    threads.join_all();
}

} // namespace parakeet_wx
