// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS

#include <vector>

namespace Utils {
std::vector<std::string> split(const std::string &sentence,
                               const std::string &separator);

std::string get_current_dir();
}  // namespace Utils

#endif