// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS

#include <vector>

namespace Utils {
std::string trim(const std::string &original);
std::vector<std::string> split(const std::string &sentence,
                               const std::string &separator);

std::string getCurrentDirectory();
void listFiles(const std::string &path, std::vector<std::string> &filePaths);

}  // namespace Utils

#endif