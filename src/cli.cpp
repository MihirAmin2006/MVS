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
    "  init [--bare]     Initialize repository\n"
    "  --status            Show repository status\n"
    "  --version           Show MVS version\n"
    "  --help            Show help message\n"
    "  --verbose         Enable debug output\n"
    "hash-object <file>    Hash file and store as object\n";
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


        throw MVSException("Unknown command: " + command);

    } catch (const MVSException& e) {
        Logger::error(e.what());
        return 1;
    }
}
