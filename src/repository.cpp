#include "repository.h"
#include "exceptions.h"
#include "logger.h"
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = filesystem;

bool Repository::exists() {
    return fs::exists(".mvs") && fs::is_directory(".mvs");
}

void Repository::requireRepository() {
    if (!exists()) {
        throw MVSException("Not an MVS repository (run 'mvs init').");
    }
}

void Repository::init(bool bare) {

    if (exists()) {
        throw MVSException("Repository already initialized.");
    }

    try {

        if (bare) {
            // Bare repo: current folder becomes repo
            fs::create_directory("objects");
            fs::create_directory("commits");
            fs::create_directory("refs");

            ofstream headFile("HEAD");
            headFile << "ref: refs/main\n";
            headFile.close();

            Logger::info("Bare repository structure created.");
        }
        else {
            // Normal repo
            fs::create_directory(".mvs");
            fs::create_directory(".mvs/objects");
            fs::create_directory(".mvs/commits");
            fs::create_directory(".mvs/refs");

            ofstream headFile(".mvs/HEAD");
            headFile << "ref: refs/main\n";
            headFile.close();

            ofstream indexFile(".mvs/index");
            indexFile.close();

            Logger::info(".mvs structure created.");
        }

    } catch (const fs::filesystem_error& e) {
        throw MVSException("Initialization failed: " + string(e.what()));
    }
}
