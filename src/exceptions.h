#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

using namespace std;

class MVSException : public runtime_error {
public:
    explicit MVSException(const string& message)
        : runtime_error(message) {}
};

#endif