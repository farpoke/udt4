
#include <iostream>
#include <string>
#include <vector>

#include "udt.hpp"

void testAddress()
{
    static const std::vector<std::string> addresses = {
        "",
        "127.0.0.1",
        "localhost",
        "127.0.0.1:8080",
        "localhost:8080",
        ":8080",
        "spacewarp.se:http"
    };
    //
    std::cout << "Testing UDT::Address..." << std::endl;
    for (auto &s : addresses)
    {
        UDT::Address addr(s);
        std::cout << "\"" << s << "\" -> " << addr.toString() << std::endl;
    }
}

int main(int, char *[])
{
    testAddress();
    return 0;
}

