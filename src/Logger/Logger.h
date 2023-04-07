#include <iostream>
class Logger {
   private:
    static std::string __filename;

   public:
    static void log(std::string message);
    static void log(std::string message, std::string filename);
};
