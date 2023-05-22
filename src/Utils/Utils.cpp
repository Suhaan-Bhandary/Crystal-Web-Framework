#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

namespace Utils {
std::string trim(const std::string &original) {
    int n = original.size();

    // Now Find the first non space in the original from start
    int nonSpaceIndexFromStart = 0;
    while (nonSpaceIndexFromStart < n &&
           original[nonSpaceIndexFromStart] == ' ') {
        nonSpaceIndexFromStart++;
    }

    // Now Find the first non space in the original from end
    int nonSpaceIndexFromEnd = original.size() - 1;
    while (nonSpaceIndexFromEnd >= 0 && original[nonSpaceIndexFromEnd] == ' ') {
        nonSpaceIndexFromEnd--;
    }

    // Start from non space till end
    std::string trimmed = "";
    for (int i = nonSpaceIndexFromStart; i <= nonSpaceIndexFromEnd; i++) {
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
