#include "index.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

void Index::load() {

    entries.clear();

    if (!fs::exists(".mvs/index"))
        throw MVSException("Index file not found.");

    ifstream file(".mvs/index");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string path, hash;
        ss >> path >> hash;
        entries[path] = hash;
    }

    file.close();
}

void Index::add(const string& filePath, const string& hash) {
    entries[filePath] = hash;
}

void Index::save() {

    ofstream file(".mvs/index");

    for (const auto& pair : entries) {
        file << pair.first << " " << pair.second << "\n";
    }

    file.close();
}
