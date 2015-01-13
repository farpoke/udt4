
#include "udt.hpp"

#include <cstring>

#include <netdb.h>

namespace UDT
{

    Exception::Exception(const std::string &msg)
        : std::runtime_error(msg + " | " + getLastError())
    {
    }

    std::string Exception::getLastError()
    {
        auto err = UDT::getlasterror();
        return std::to_string(err.getErrorCode()) + ": " + err.getErrorMessage();
    }



    InitGuard::InitGuard()
    {
        if (UDT::startup() == UDT::ERROR)
            throw Exception("UDT::startup failed.");
    }

    InitGuard::~InitGuard()
    {
        UDT::cleanup();
    }



    Address::Address()
    {
    }

    Address::Address(const std::string &address)
    {
        auto split = address.find_first_of(':');
        auto name = address.substr(0, split);
        auto service = (split != std::string::npos ? address.substr(split + 1) : "");
        //
        if (name == "")
            name = "localhost";
        //
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        //
        addrinfo *info;
        auto res = getaddrinfo(name.c_str(), service.c_str(), &hints, &info);
        if (res != 0)
            throw std::runtime_error(std::string("getaddrinfo failed, ") + gai_strerror(res));
        //
        if (info == nullptr)
            throw std::runtime_error("getaddrinfo returned no result.");
        //
        memcpy(&addr, info->ai_addr, sizeof(addr));
        freeaddrinfo(info);
    }

    Address::Address(const sockaddr &sa)
    {
        memcpy(&addr, &sa, sizeof(addr));
    }

    std::string Address::toString() const
    {
        auto port = ntohs(addr.sin_port);
        return std::string(inet_ntoa(addr.sin_addr)) + ":" + std::to_string(port);
    }

    Address::operator sockaddr() const
    {
        sockaddr sa;
        memcpy(&sa, &addr, sizeof(sa));
        return sa;
    }

    Address::operator const sockaddr * () const
    {
        return (const sockaddr*)&addr;
    }



    Socket::Socket()
        : socket(UDT::INVALID_SOCK)
    {
    }

    Socket::Socket(int af, int type)
    {
        socket = UDT::socket(af, type, 0);
        if (socket == UDT::INVALID_SOCK)
            throw Exception("UDT::socket failed.");
    }

    Socket::Socket(UDTSOCKET s)
        : socket(s)
    {
        if (socket == UDT::INVALID_SOCK)
            throw std::logic_error("Socket given invalid UDTSOCKET.");
    }

    Socket::Socket(Socket &rhs)
        : socket(rhs.socket)
    {
        rhs.socket = UDT::INVALID_SOCK;
    }

    Socket::Socket(Socket &&move)
        : socket(move.socket)
    {
        move.socket = UDT::INVALID_SOCK;
    }

    Socket::~Socket()
    {
        close();
    }

    Socket & Socket::operator= (Socket &rhs)
    {
        if (socket != UDT::INVALID_SOCK)
            UDT::close(socket);
        socket = rhs.socket;
        rhs.socket = UDT::INVALID_SOCK;
        return *this;
    }

    Socket & Socket::operator= (Socket &&move)
    {
        if (socket != UDT::INVALID_SOCK)
            UDT::close(socket);
        socket = move.socket;
        move.socket = UDT::INVALID_SOCK;
        return *this;
    }

    bool Socket::isValid() const
    {
        return socket != UDT::INVALID_SOCK;
    }

    Socket::operator bool () const
    {
        return isValid();
    }

#define ASSERT_VALID if (!isValid()) throw std::logic_error("Socket is invalid.");

    Socket Socket::accept(Address *addr)
    {
        ASSERT_VALID
        //
        sockaddr sa;
        int sa_len = sizeof(sa);
        auto s = UDT::accept(socket, &sa, &sa_len);
        if (s == UDT::INVALID_SOCK)
            throw Exception("UDT::accept failed.");
        if (addr != nullptr)
            *addr = Address(sa);
        return Socket(s);
    }

    void Socket::bind(const Address &addr)
    {
        ASSERT_VALID
        if (UDT::bind(socket, addr, sizeof(sockaddr)) == UDT::ERROR)
            throw Exception("UDT::bind failed.");
    }

    void Socket::close()
    {
        if (isValid())
        {
            UDT::close(socket);
            socket = UDT::INVALID_SOCK;
        }
    }

    void Socket::connect(const Address &addr)
    {
        ASSERT_VALID
        if (UDT::connect(socket, addr, sizeof(sockaddr)) == UDT::ERROR)
            throw Exception("UDT::connect failed.");
    }

    void Socket::listen(int backlog)
    {
        ASSERT_VALID
        if (UDT::listen(socket, backlog) == UDT::ERROR)
            throw Exception("UDT::listen failed.");
    }

}

