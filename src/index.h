#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <unordered_map>

using namespace std;

class Index {
public:
    void add(const string& filePath, const string& hash);
    void load();
    void save();

private:
    unordered_map<string, string> entries;
};

#endif
