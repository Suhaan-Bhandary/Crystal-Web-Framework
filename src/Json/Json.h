#ifndef INCLUDED_JSON
#define INCLUDED_JSON

#include "Node/Node.h"

namespace Json {
// Wrapper around node
class Json {
   public:
    Json();
    Json(const char* str, bool readFromFile = false);

    Json(const Json& otherJson);
    Json& operator=(const Json& otherJson);

    Json(Json&& otherJson);
    Json& operator=(Json&& otherJson);

    Json(const Node& otherNode);
    Json& operator=(const Node& otherNode);

    ~Json();

    Node& getData();

   private:
    Node* data;
};

}  // namespace Json

#endif