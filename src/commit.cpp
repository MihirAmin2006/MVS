#include "commit.h"
#include "index.h"
#include "hashing.h"
#include "repository.h"
#include "exceptions.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;
namespace fs = filesystem;

string Commit::create(const string& message) {

    Repository repo;
    repo.requireRepository();

    Index index;
    index.load();

    // Read index content
    ifstream indexFile(".mvs/index");
    stringstream indexContent;
    indexContent << indexFile.rdbuf();
    indexFile.close();

    if (indexContent.str().empty())
        throw MVSException("Nothing to commit.");

    // Get parent commit if exists
    string parent;
    if (fs::exists(".mvs/HEAD")) {
        ifstream headFile(".mvs/HEAD");
        getline(headFile, parent);
        headFile.close();
    }

    // Timestamp
    time_t now = time(nullptr);

    // Create commit content
    stringstream commitData;
    commitData << "parent " << parent << "\n";
    commitData << "date " << now << "\n";
    commitData << "\n";
    commitData << message << "\n";
    commitData << "\n";
    commitData << indexContent.str();

    string commitString = commitData.str();

    // Hash commit
    string commitHash = Hashing::sha1(commitString);

    // Store commit object
    string dir = commitHash.substr(0, 2);
    string filename = commitHash.substr(2);

    string objectDir = ".mvs/objects/" + dir;

    if (!fs::exists(objectDir))
        fs::create_directory(objectDir);

    ofstream out(objectDir + "/" + filename);
    out << commitString;
    out.close();

    // Update HEAD
    ofstream headFile(".mvs/HEAD");
    headFile << commitHash;
    headFile.close();

    // Clear index
    ofstream clearIndex(".mvs/index");
    clearIndex.close();

    return commitHash;
}
