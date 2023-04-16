#include <unistd.h>

#include <string>
#include <vector>

namespace Utils {
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

std::string get_current_dir() {
    char buff[FILENAME_MAX];  // create string buffer to hold path
    getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}
}  // namespace Utils
