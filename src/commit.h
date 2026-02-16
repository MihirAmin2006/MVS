#ifndef COMMIT_H
#define COMMIT_H

#include <string>

using namespace std;

class Commit {
public:
    static string create(const string& message);
};

#endif
