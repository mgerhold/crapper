#pragma once

#include <future>
#include <gtest/gtest.h>
#include <deque>
#include <sockets/detail/synchronized.hpp>
#include <sockets/sockets.hpp>
#include <crapper/crapper.hpp>

class CrapperFixture : public testing::Test {
private:
    std::promise<std::uint16_t> m_socket_port_promise;
    std::future<std::uint16_t> m_socket_port_future;
    std::promise<std::string> m_http_port_promise;
    std::future<std::string> m_http_port_future;
    c2k::Synchronized<std::deque<std::string>> m_received_messages;
    std::condition_variable_any m_condition_variable;
    std::jthread m_socket_thread;
    std::jthread m_python_thread;

    void run_socket_thread();

protected:
    std::string http_port;
    std::string base_url;

    CrapperFixture();

    ~CrapperFixture() override;

    static void SetUpTestSuite();

    [[nodiscard]] nlohmann::json get_next_request();
};
