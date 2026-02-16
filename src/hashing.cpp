#include "hashing.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using namespace std;

string Hashing::sha1(const string& data) {

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()),
         data.size(),
         hash);

    stringstream ss;

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0')
           << static_cast<int>(hash[i]);
    }

    return ss.str();
}
