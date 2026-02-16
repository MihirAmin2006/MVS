#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>

class Repository {
public:
    void init(bool bare);
    bool exists();
    void requireRepository();
};

#endif
