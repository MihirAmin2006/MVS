#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

using namespace std;

class Logger {
    public:
        static bool verbose;

        static void info(const string& msg) {
            if (verbose)
                cout << "[INFO] " << msg << "\n";
        }

        static void error(const string& msg) {
            cerr << "[ERROR] " << msg << "\n";
        }
};

#endif