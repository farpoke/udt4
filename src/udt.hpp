#ifndef _UDT_HPP_
#define _UDT_HPP_

#include <stdexcept>

#include <arpa/inet.h>

#include "udt.h"

namespace UDT
{
    
    class Exception : std::runtime_error
    {
    public:
        Exception(const std::string &msg);

        static std::string getLastError();
    };

    class InitGuard
    {
    public:
        InitGuard();
        ~InitGuard();
    };

    class Address
    {
        sockaddr_in addr = {0, 0, {0}, {0}};

    public:
        Address();
        Address(const std::string &address);
        Address(const sockaddr &sa);

        std::string toString() const;
       
        operator sockaddr () const;
        operator const sockaddr * () const;

    };

    class Socket
    {
        UDTSOCKET socket;

    public:
        Socket();
        Socket(int af, int type);
        Socket(UDTSOCKET s);
        Socket(Socket &rhs);
        Socket(Socket &&move);
        ~Socket();

        Socket & operator= (Socket &rhs);
        Socket & operator= (Socket &&move);

        bool isValid() const;
        operator bool () const;

        Socket accept(Address *addr=nullptr);
        void bind(const Address &addr);
        void close();
        void connect(const Address &addr);
        void listen(int backlog);

    };

}

#endif
