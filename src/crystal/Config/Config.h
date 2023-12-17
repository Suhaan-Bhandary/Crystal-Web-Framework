// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_CONFIG
#define INCLUDED_CONFIG

#include <string>

namespace Crystal {
class Config {
   public:
    static std::string TEMPLATE_DIR_PATH;
    static std::string PUBLIC_DIR_PATH;
};
}  // namespace Crystal

#endif