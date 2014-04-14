/**
 * Unix and GAI (getaddrinfo) error convenience exceptions
 *
 * @author Yichen Zhao
 * @remarks Exported from (private) UDP packet obfuscation project for
 *          convenience
 */
#pragma once
#include <stdexcept>
#include <string>

class unix_error: public std::runtime_error {
    const std::string get_error_message();
 public:
    unix_error();
    unix_error(const std::string& message);
};

class gai_unix_error: public std::runtime_error {
    const std::string get_error_message(int errcode);
 public:
    gai_unix_error(int errcode);
    gai_unix_error(int errcode, const std::string& message);
};
