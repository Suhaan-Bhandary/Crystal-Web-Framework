// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS

#include <string>
#include <vector>

namespace Utils {
std::string trim(const std::string &original);
std::vector<std::string> split(const std::string &sentence,
                               const std::string &separator);

std::string getCurrentDirectory();
void listFiles(const std::string &path, std::vector<std::string> &filePaths);

// Get file size
int getFileSizeInBytes(const std::string &filePath);
std::string getLastModifiedTimeStamp(const std::string &filePath);
std::string getFileETag(const std::string &filePath);

// Function to Clean Query String from special char
std::string cleanEncodedString(std::string &value);
}  // namespace Utils

#endif