#include "log.h"
#include "repository.h"
#include "exceptions.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;
namespace fs = filesystem;

static string readObject(const string& hash) {

    string dir = hash.substr(0, 2);
    string filename = hash.substr(2);

    string path = ".mvs/objects/" + dir + "/" + filename;

    if (!fs::exists(path))
        throw MVSException("Corrupted commit object: " + hash);

    ifstream file(path);
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void Log::show() {

    Repository repo;
    repo.requireRepository();

    if (!fs::exists(".mvs/HEAD"))
        throw MVSException("No commits yet.");

    ifstream headFile(".mvs/HEAD");
    string currentHash;
    getline(headFile, currentHash);
    headFile.close();

    if (currentHash.empty()) {
        cout << "No commits yet.\n";
        return;
    }

    while (!currentHash.empty()) {

        string content = readObject(currentHash);
        stringstream ss(content);

        string line;
        string parent;
        string date;
        string message;

        // Parse commit metadata
        getline(ss, line); // parent
        parent = line.substr(7);

        getline(ss, line); // date
        long timestamp = stol(line.substr(5));

        getline(ss, line); // empty line

        getline(ss, message); // commit message

        // Convert timestamp
        time_t time = timestamp;
        string dateStr = ctime(&time);

        cout << "commit " << currentHash << "\n";
        cout << "Date: " << dateStr;
        cout << "    " << message << "\n\n";

        currentHash = parent;
    }
}