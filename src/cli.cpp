#include "index.h"
#include "commit.h"
#include "cli.h"
#include "object_store.h"
#include "repository.h"
#include "exceptions.h"
#include "config.h"
#include "logger.h"
#include <iostream>
#include <string>


using namespace std;

static void printHelp() {
    cout <<
    "MVS - Mihir Version System\n\n"
    "Usage:\n"
    "  mvs <command> [options]\n\n"
    "Commands:\n"
    "  init [--bare]         Initialize repository\n"
    "  --status              Show repository status\n"
    "  --version             Show MVS version\n"
    "  --help                Show help message\n"
    "  --verbose             Enable debug output\n"
    "  hash-object <file>    Hash file and store as object\n"
    "  add <file>            Add file to staging area\n";
}

int CLI::run(int argc, char* argv[]) {

    if (argc < 2) {
        printHelp();
        return 1;
    }

    string command = argv[1];

    // Check for global verbose flag
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--verbose") {
            Logger::verbose = true;
        }
    }

    Repository repo;

    try {

        if (command == "--help" || command == "-h") {
            printHelp();
            return 0;
        }

        if (command == "-v" || command == "--version") {
            cout << "MVS version " << MVS_VERSION << "\n";
            return 0;
        }

        if (command == "init") {

            bool bare = false;

            for (int i = 2; i < argc; i++) {
                string arg = argv[i];
                if (arg == "--bare") bare = true;
            }

            repo.init(bare);

            if (bare)
                cout << "Initialized empty bare MVS repository.\n";
            else
                cout << "Initialized empty MVS repository.\n";

            return 0;
        }

        if (command == "--status") {

            repo.requireRepository();

            cout << "On branch main\n";
            cout << "No commits yet\n";

            Logger::info("Status checked successfully.");

            return 0;
        }
        
        if (command == "hash-object") {
            if (argc < 3)
                throw MVSException("Usage: mvs hash-object <file>");

            string filePath = argv[2];

            string hash = ObjectStore::hashAndStore(filePath);

            cout << hash << "\n";
            return 0;
        }

        if (command == "add") {
            repo.requireRepository();

            if (argc < 3)
                throw MVSException("Usage: mvs add <file>");

            string filePath = argv[2];

            // Hash and store object
            string hash = ObjectStore::hashAndStore(filePath);

            // Load index
            Index index;
            index.load();

            // Add file entry
            index.add(filePath, hash);

            // Save index
            index.save();

            cout << "Added " << filePath << "\n";
            return 0;
        }

        if (command == "commit") {

            repo.requireRepository();

            if (argc < 4 || std::string(argv[2]) != "-m")
                throw MVSException("Usage: mvs commit -m \"message\"");

            std::string message = argv[3];

            std::string hash = Commit::create(message);

            std::cout << "[main " << hash.substr(0,7) << "] "
                    << message << "\n";

            return 0;
        }



        throw MVSException("Unknown command: " + command);

    } catch (const MVSException& e) {
        Logger::error(e.what());
        return 1;
    }
}
