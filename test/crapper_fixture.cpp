#include "crapper_fixture.hpp"

#ifndef TEST_ROOT
#error "Test root must be defined"
#endif

template<typename... Args>
[[nodiscard]] static int execute_command(char const* const format_string, Args&&... args) {
    auto const command = std::vformat(format_string, std::make_format_args(std::forward<Args>(args)...));
    return std::system(command.c_str());
}

enum class OperatingSystem {
    Windows,
    Linux,
};

#ifdef _WIN32
static constexpr auto operating_system = OperatingSystem::Windows;
#else
static constexpr auto operating_system = OperatingSystem::Linux;
#endif

static void setup_python_venv() {
    if constexpr (operating_system == OperatingSystem::Windows) {
        if (execute_command("{}/tools/setup_venv.bat {}/tools/http-server/requirements.txt", TEST_ROOT, TEST_ROOT)
            != EXIT_SUCCESS) {
            throw std::runtime_error{ "unable to setup virtual environment" };
        }
    } else {
        assert(operating_system == OperatingSystem::Linux);
        if (execute_command("bash {}/tools/setup_venv.sh {}/tools/http-server/requirements.txt", TEST_ROOT, TEST_ROOT)
            != EXIT_SUCCESS) {
            throw std::runtime_error{ "unable to setup virtual environment" };
        }
    }
}

static void run_http_server(std::uint16_t const socket_port) {
    if constexpr (operating_system == OperatingSystem::Windows) {
        if (execute_command(
                    "{}/tools/run_http_server.bat {}/tools/http-server/main.py {}",
                    TEST_ROOT,
                    TEST_ROOT,
                    socket_port
            )
            != EXIT_SUCCESS) {
            throw std::runtime_error{ "unable to execute http-server script" };
        }
    } else {
        assert(operating_system == OperatingSystem::Linux);
        if (execute_command(
                    "bash {}/tools/run_http_server.sh {}/tools/http-server/main.py {}",
                    TEST_ROOT,
                    TEST_ROOT,
                    socket_port
            )
            != EXIT_SUCCESS) {
            throw std::runtime_error{ "unable to execute http-server script" };
        }
    }
}

[[nodiscard]] std::optional<std::string> read_until_null_terminator(c2k::ClientSocket& peer) {
    auto buffer = std::string{};
    while (true) {
        try {
            auto const c = peer.receive<char>().get();
            if (c == '\0') {
                break;
            }
            buffer += c;
        } catch (c2k::TimeoutError const&) {
            return std::nullopt;
        } catch (c2k::ReadError const&) {
            return std::nullopt;
        }
    }
    return buffer;
}

void CrapperFixture::run_socket_thread() {
    auto peer_promise = std::promise<c2k::ClientSocket>{};
    auto peer_future = peer_promise.get_future();
    auto socket = c2k::Sockets::create_server(c2k::AddressFamily::Ipv4, 0, [&peer_promise](c2k::ClientSocket client) {
        peer_promise.set_value(std::move(client));
    });
    m_socket_port_promise.set_value(socket.local_address().port);
    auto peer = std::move(peer_future).get();

    // the first data we receive through the socket is the port of the http server (as null-terminated string)
    m_http_port_promise.set_value(read_until_null_terminator(peer).value());

    while (true) {
        auto received = read_until_null_terminator(peer);
        if (not received.has_value()) {
            break;
        }
        m_received_messages.apply([data = std::move(received).value()](std::deque<std::string>& messages) mutable {
            messages.push_back(std::move(data));
        });
        m_condition_variable.notify_one();
    }
}
CrapperFixture::CrapperFixture()
    : m_socket_port_future{ m_socket_port_promise.get_future() },
      m_http_port_future{ m_http_port_promise.get_future() },
      m_received_messages{ std::deque<std::string>{} },
      m_socket_thread{ [this] { run_socket_thread(); } },
      m_python_thread{ [socket_port = m_socket_port_future.get()]() { run_http_server(socket_port); } } {
    http_port = m_http_port_future.get();
    base_url = std::format("http://127.0.0.1:{}", http_port);
}
CrapperFixture::~CrapperFixture() {
    std::ignore = Crapper{}.post(std::format("{}/shutdown", base_url)).send();
}
void CrapperFixture::SetUpTestSuite() {
    setup_python_venv();
}
nlohmann::json CrapperFixture::get_next_request() {
    // todo: parse into custom data structure
    return nlohmann::json::parse(m_received_messages.wait_and_apply(
            m_condition_variable,
            [](auto const& messages) { return not messages.empty(); },
            [](std::deque<std::string>& messages) {
                auto result = messages.back();
                messages.pop_back();
                return result;
            }
    ));
}
