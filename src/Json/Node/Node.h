#ifndef INCLUDED_JSON_NODE
#define INCLUDED_JSON_NODE

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

enum JsonType {
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    FRACTION,
    BOOL,
    NULL_,
};

namespace Json {
// Forward declaration of Node
struct Node;

// Defining types for Json
using Object = std::unordered_map<std::string, Node *>;
using Array = std::vector<Node *>;
using String = std::string;
using Number = long long;
using Fraction = double;
using Bool = bool;

struct Node {
   public:
    Node();
    ~Node();

    Node(const Node &otherNode);
    Node &operator=(const Node &otherNode);

    Node(Node &&otherNode);
    Node &operator=(Node &&otherNode);

    // Getters
    JsonType getType() const;

    template <typename T>
    const T &get() const;

    Node &get(int index) const;
    Node &get(const String &key) const;

    // clears the node
    void clear();

    void set(Object value);
    void set(Array value);
    void set(String value);
    void set(Number value);
    void set(Fraction value);
    void set(Bool value);
    void set(const char *value);

    void set(const std::string &key, Object value);
    void set(const std::string &key, Array value);
    void set(const std::string &key, String value);
    void set(const std::string &key, Number value);
    void set(const std::string &key, Fraction value);
    void set(const std::string &key, Bool value);
    void set(const std::string &key, Node *value);
    void set(const std::string &key, const char *value);

    void push(Object value);
    void push(Array value);
    void push(String value);
    void push(Number value);
    void push(Fraction value);
    void push(Bool value);
    void push(Node *value);
    void push(const char *value);

    bool isNull();

    std::string toString(int level = 0, int spaces = 4);

   private:
    JsonType type;
    std::variant<String, Number, Fraction, Bool, Array, Object> value;
};
}  // namespace Json

#endif
