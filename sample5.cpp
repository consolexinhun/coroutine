#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <cstring>
#include <string>

#include <libgo/coroutine.h>

static constexpr uint16_t port = 43332;


namespace asio = ::boost::asio;
namespace asio_ip = ::boost::asio::ip;

using ::boost::system::error_code;

void echo_server(asio_ip::tcp::endpoint const& addr);
void client(asio_ip::tcp::endpoint const& addr);

asio::io_service ios;

int main(int argc, char** argv) {
    for (int i = 0; i < 5; i++) {
        // test::asio_ip::tcp::endpoint addr
        asio_ip::tcp::endpoint addr(asio_ip::address::from_string("127.0.0.1"), port+i);
        go [addr] {
            echo_server(addr);
        };
        go [addr] {
            client(addr);
        };
    }

    std::thread([] {
        co_sleep(200);
        co_sched.Stop();
    }).detach();

    co_sched.Start();
    return 0;
}

void echo_server(asio_ip::tcp::endpoint const& addr) {
    asio_ip::tcp::acceptor acc(ios, addr, true);
    for (int i = 0; i < 2; i++) {
        std::shared_ptr<asio_ip::tcp::socket> s(new asio_ip::tcp::socket(ios));
        error_code ec;
        acc.accept(*s, ec);
        if (ec) {
            printf("line:%d accept error:%s\n", __LINE__, ec.message().c_str());
            return;
        }

        go [s] {
            char buf[1024];
            error_code ec;
            auto n = s->read_some(asio::buffer(buf), ec);
            if (ec) {
                printf("line:%d accept error:%s\n", __LINE__, ec.message().c_str());
                return;
            }

            n = s->write_some(asio::buffer(buf, n), ec);
            if (ec) {
                printf("line:%d accept error:%s\n", __LINE__, ec.message().c_str());
                return;
            }

            error_code ignore_ec;
            n = s->read_some(asio::buffer(buf, 1), ignore_ec);
        };
    }
}
void client(asio_ip::tcp::endpoint const& addr) {
    asio_ip::tcp::socket s(ios);
    printf("start connect\n");

    error_code ec;
    s.connect(addr, ec);
    if (ec) {
        printf("line:%d connect error:%s\n", __LINE__, ec.message().c_str());
        return;
    }

    printf("connected success\n");

    std::string msg = "1234";
    int n = s.write_some(asio::buffer(msg), ec);
    if (ec) {
        printf("line:%d write_some error:%s\n", __LINE__, ec.message().c_str());
        return;
    }

    printf("client send msg [%d] %s\n", (int)msg.size(), msg.c_str());
    char buf[12];
    n = s.receive(asio::buffer(buf, n));
    printf("client receive msg [%d] %.*s\n", n, n, buf);

}


