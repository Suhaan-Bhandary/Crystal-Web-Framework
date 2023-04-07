#include <iostream>
class Logger {
   private:
    static std::string __filename;

   public:
    static void log(const std::string &message);
    static void log(const std::string &message, const std::string &filename);
};
