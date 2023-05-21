// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_JSON
#define INCLUDED_JSON

#include <string>

namespace Json {
class Json {
   private:
   public:
    Json(const std::string &body);
};
}  // namespace Json

#endif