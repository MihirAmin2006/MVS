#include "object_store.h"
#include "hashing.h"
#include "repository.h"
#include "exceptions.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = filesystem;

string ObjectStore::hashAndStore(const string& filePath) {

    Repository repo;
    repo.requireRepository();

    if (!fs::exists(filePath)) {
        throw MVSException("File does not exist: " + filePath);
    }

    // Read file
    ifstream file(filePath, ios::binary);
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Hash content
    string hash = Hashing::sha1(content);

    // Split hash
    string dir = hash.substr(0, 2);
    string filename = hash.substr(2);

    string objectDir = ".mvs/objects/" + dir;

    if (!fs::exists(objectDir))
        fs::create_directory(objectDir);

    string objectPath = objectDir + "/" + filename;

    if (!fs::exists(objectPath)) {
        ofstream out(objectPath, ios::binary);
        out << content;
        out.close();
    }

    return hash;
}
