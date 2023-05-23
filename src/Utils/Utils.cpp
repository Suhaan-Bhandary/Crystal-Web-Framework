#include <dirent.h>
#include <unistd.h>

#include <string>
#include <unordered_set>
#include <vector>

namespace Utils {
// The trim is used to all \r\n, \n and " " from both sides
std::string trim(const std::string &original) {
    int n = original.size();

    // chars to trim
    std::unordered_set<char> charsToTrim = {' ', '\n', '\r'};

    // Now Find the first non space in the original from start
    int nonTrimFromStart = 0;
    while (nonTrimFromStart < n &&
           charsToTrim.count(original[nonTrimFromStart]) == 1) {
        nonTrimFromStart++;
    }

    // Now Find the first non space in the original from end
    int nonTrimFromEnd = original.size() - 1;
    while (nonTrimFromEnd >= 0 &&
           charsToTrim.count(original[nonTrimFromEnd]) == 1) {
        nonTrimFromEnd--;
    }

    // Start from non space till end
    std::string trimmed = "";
    for (int i = nonTrimFromStart; i <= nonTrimFromEnd; i++) {
        trimmed.push_back(original[i]);
    }

    return trimmed;
}

std::vector<std::string> split(const std::string &sentence,
                               const std::string &separator) {
    std::string currentToken = "";
    std::vector<std::string> tokens;

    int i = 0;
    while (i < sentence.size()) {
        // Check it matches with separator
        bool isValidSeparator = true;
        for (int k = 0; k < separator.size() && i + k < sentence.size(); k++) {
            if (sentence[i + k] != separator[k]) {
                isValidSeparator = false;
                break;
            }
        }

        if (isValidSeparator) {
            i = i + separator.size();
            tokens.push_back(currentToken);
            currentToken.clear();
        } else {
            currentToken.push_back(sentence[i]);
            i++;
        }
    }

    if (currentToken.size() != 0) {
        tokens.push_back(currentToken);
    }

    return tokens;
}

std::string getCurrentDirectory() {
    char buff[FILENAME_MAX];  // create string buffer to hold path
    getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

void listFiles(const std::string &path, std::vector<std::string> &filePaths) {
    if (auto dir = opendir(path.c_str())) {
        while (auto f = readdir(dir)) {
            if (!f->d_name || f->d_name[0] == '.') continue;

            if (f->d_type == DT_DIR) {
                listFiles(path + f->d_name + "/", filePaths);
            }

            if (f->d_type == DT_REG) {
                std::string filePath = path;
                filePath += f->d_name;
                filePaths.push_back(filePath);
            }
        }
        closedir(dir);
    }
}
}  // namespace Utils
