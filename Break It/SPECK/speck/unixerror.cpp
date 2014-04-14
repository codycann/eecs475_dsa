/**
 * Unix and GAI (getaddrinfo) error convenience exceptions
 *
 * @author Yichen Zhao
 * @remarks Exported from (private) UDP packet obfuscation project for
 *          convenience
 */
#include <string.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <netdb.h>
#include "./unixerror.h"

using std::string;
using std::ostringstream;

const string unix_error::get_error_message() {
    auto err = errno;
    errno = 0;
    ostringstream result;

    result << "Error 0x" << std::setw(8) << std::setfill('0') << std::hex << err
           << ": " << strerror(err);

    if (errno != 0) {
        //Failed strerror
        perror("Could not get error message");
        return "Could not get error message";
    }

    return result.str();
}

unix_error::unix_error(): std::runtime_error(get_error_message()) {
}

unix_error::unix_error(const string& message)
: std::runtime_error(message + ": " + get_error_message()) {
}

const string gai_unix_error::get_error_message(int errcode) {
    ostringstream result;

    result << "GAI Error 0x" << std::setw(8) << std::setfill('0') << std::hex
           << errcode << ": " << gai_strerror(errcode);

    return result.str();
}

gai_unix_error::gai_unix_error(int errcode)
: std::runtime_error(get_error_message(errcode)) {
}

gai_unix_error::gai_unix_error(int errcode, const string& message)
: std::runtime_error(message + ": " + get_error_message(errcode)) {
}
