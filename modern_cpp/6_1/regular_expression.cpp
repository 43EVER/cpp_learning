#include <iostream>
#include <algorithm>
#include <regex>
#include <string>
#include <time.h>
#include <cstdlib>

std::string fnames[] = {
    "foo.txt",
    "bar.txt",
    "a.txt",
    "a0.txt",
    "AAA.txt"
};

void simple() {

    std::regex txt_regex("[a-z]+\\.txt");
    for (auto fname: fnames) {
        std::cout << fname << ": " << std::regex_match(fname, txt_regex) << std::endl;
    }
}

int main() {
    std::regex base_regex("([a-z]+)\\.txt");
    std::smatch base_match;
    for (const auto& fname : fnames) {
        if (std::regex_match(fname, base_match, base_regex)) {
            if (base_match.size() == 2) {
                std::string base = base_match[1].str();
                std::cout << "sub-match[0]: " << base_match[0].str() << std::endl;
                std::cout << fname << " sub-match[1]: " << base << std::endl;
            }
        }
    }
}