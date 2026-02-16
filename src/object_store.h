#ifndef OBJECT_STORE_H
#define OBJECT_STORE_H

#include <string>

using namespace std;

class ObjectStore {
public:
    static string hashAndStore(const string& filePath);
};

#endif
