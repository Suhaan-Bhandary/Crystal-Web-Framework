#include "Node.h"

#include <stdexcept>

Crystal::Json::Node::Node() { type = NULL_; }

Crystal::Json::Node::Node(Object value) {
    type = OBJECT;
    this->value = value;
}

Crystal::Json::Node::Node(Array value) {
    type = ARRAY;
    this->value = value;
}

Crystal::Json::Node::Node(String value) {
    type = STRING;
    this->value = value;
}

Crystal::Json::Node::Node(Number value) {
    type = NUMBER;
    this->value = value;
}

Crystal::Json::Node::Node(UnsignedNumber value) {
    type = UNSIGNED_NUMBER;
    this->value = value;
}

Crystal::Json::Node::Node(Fraction value) {
    type = FRACTION;
    this->value = value;
}

Crystal::Json::Node::Node(Bool value) {
    type = BOOL;
    this->value = value;
}

Crystal::Json::Node::Node(const char* value) {
    type = STRING;
    this->value = std::string(value);
}

Crystal::Json::Node::Node(const Node& otherNode) {
    // Set initial type as null
    type = JsonType::NULL_;

    switch (otherNode.type) {
        case ARRAY: {
            const Array& oldArray = std::get<Array>(otherNode.value);
            set(Array());
            for (auto& node : oldArray) {
                if (!node) continue;
                push(new Node(*node));
            }
            break;
        }
        case OBJECT: {
            const Object& oldObject = std::get<Object>(otherNode.value);
            set(Object());
            for (auto& p : oldObject) {
                if (!p.second) continue;
                set(p.first, new Node(*p.second));
            }
            break;
        }
        default:
            type = otherNode.type;
            value = otherNode.value;
            break;
    }
}

Crystal::Json::Node& Crystal::Json::Node::operator=(const Node& otherNode) {
    switch (otherNode.type) {
        case ARRAY: {
            const Array& oldArray = std::get<Array>(otherNode.value);
            set(Array());
            for (auto& node : oldArray) {
                if (!node) continue;
                push(new Node(*node));
            }
            break;
        }
        case OBJECT: {
            const Object& oldObject = std::get<Object>(otherNode.value);
            set(Object());
            for (auto& p : oldObject) {
                if (!p.second) continue;
                set(p.first, new Node(*p.second));
            }
            break;
        }
        default:
            type = otherNode.type;
            value = otherNode.value;
            break;
    }

    return *this;
}

Crystal::Json::Node::Node(Node&& otherNode) {
    type = otherNode.type;
    value = otherNode.value;
    otherNode.type = NULL_;
}

Crystal::Json::Node& Crystal::Json::Node::operator=(Node&& otherNode) {
    type = otherNode.type;
    value = otherNode.value;
    otherNode.type = NULL_;
    return *this;
}

Crystal::Json::Node::~Node() { clear(); }

JsonType Crystal::Json::Node::getType() const { return type; }

// Getters
template <>
const Crystal::Json::String& Crystal::Json::Node::get<Crystal::Json::String>()
    const {
    return std::get<Crystal::Json::String>(value);
}

template <>
const Crystal::Json::Number& Crystal::Json::Node::get<Crystal::Json::Number>()
    const {
    return std::get<Crystal::Json::Number>(value);
}

template <>
const Crystal::Json::Fraction&
Crystal::Json::Node::get<Crystal::Json::Fraction>() const {
    return std::get<Crystal::Json::Fraction>(value);
}

template <>
const Crystal::Json::Bool& Crystal::Json::Node::get<Crystal::Json::Bool>()
    const {
    return std::get<Crystal::Json::Bool>(value);
}

template <>
const Crystal::Json::Array& Crystal::Json::Node::get<Crystal::Json::Array>()
    const {
    return std::get<Array>(value);
}

template <>
const Crystal::Json::Object& Crystal::Json::Node::get<Crystal::Json::Object>()
    const {
    return std::get<Object>(value);
}

Crystal::Json::Node& Crystal::Json::Node::get(int index) const {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    if (index >= std::get<Array>(value).size()) {
        throw std::invalid_argument("Json Array out of index");
    }

    return *std::get<Array>(value).at(index);
}

Crystal::Json::Node& Crystal::Json::Node::get(const String& key) const {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    return *std::get<Object>(value).at(key);
}

// Function to clear the node and set it as null
void Crystal::Json::Node::clear() {
    switch (type) {
        case ARRAY: {
            // Delete each element in the array
            Array& array = std::get<Array>(value);
            for (auto& node : array) {
                delete node;
            }
            array.clear();
            break;
        }
        case OBJECT: {
            // Delete each value of object
            Object& object = std::get<Object>(value);
            for (auto& pair : object) {
                delete pair.second;
            }
            object.clear();
            break;
        }
    }

    type = NULL_;
}

// Setters
void Crystal::Json::Node::set(Object value) {
    clear();
    type = OBJECT;
    this->value = value;
}

void Crystal::Json::Node::set(Array value) {
    clear();
    type = ARRAY;
    this->value = value;
}

void Crystal::Json::Node::set(String value) {
    clear();
    type = STRING;
    this->value = value;
}

void Crystal::Json::Node::set(Number value) {
    clear();
    type = NUMBER;
    this->value = value;
}

void Crystal::Json::Node::set(Fraction value) {
    clear();
    type = FRACTION;
    this->value = value;
}

void Crystal::Json::Node::set(Bool value) {
    clear();
    type = BOOL;
    this->value = value;
}

void Crystal::Json::Node::set(const Node& value) {
    clear();
    (*this) = Node(value);
}

void Crystal::Json::Node::set(const char* value) {
    clear();
    type = STRING;
    this->value = std::string(value);
}

// Object Setters
void Crystal::Json::Node::set(const std::string& key, Object value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Crystal::Json::Node::set(const std::string& key, Array value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Crystal::Json::Node::set(const std::string& key, String value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Crystal::Json::Node::set(const std::string& key, Number value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Crystal::Json::Node::set(const std::string& key, Fraction value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Crystal::Json::Node::set(const std::string& key, Bool value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Crystal::Json::Node::set(const std::string& key, const Node& value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    object[key] = new Node(value);
}

void Crystal::Json::Node::set(const std::string& key, Node* value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    object[key] = value;
}

void Crystal::Json::Node::set(const std::string& key, const char* value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

// Push in array
void Crystal::Json::Node::push(Object value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Crystal::Json::Node::push(Array value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Crystal::Json::Node::push(String value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Crystal::Json::Node::push(Number value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Crystal::Json::Node::push(Fraction value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Crystal::Json::Node::push(Bool value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Crystal::Json::Node::push(const Node& value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    array.push_back(new Node(value));
}

void Crystal::Json::Node::push(Node* value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    array.push_back(value);
}

void Crystal::Json::Node::push(const char* value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

// NULL check
bool Crystal::Json::Node::isNull() { return type == NULL_; }

bool Crystal::Json::Node::contains(const std::string& key) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    return object.count(key) != 0;
}

// String representation
std::string Crystal::Json::Node::toString(int level, int spaces) {
    std::string indentation = std::string(level * spaces, ' ');
    std::string nextLevelIndentation = std::string((level + 1) * spaces, ' ');

    switch (type) {
        case OBJECT: {
            Object& object = std::get<Object>(value);

            std::string str = "";
            str += "{\n";

            int counter = 0;
            for (auto& p : object) {
                counter++;

                str += nextLevelIndentation;
                str += "\"" + p.first + "\": ";
                str += p.second->toString(level + 1, spaces);

                if (counter != object.size()) {
                    str += ",\n";
                }
            }

            str += "\n" + indentation + "}";
            return str;
        }
        case ARRAY: {
            Array& array = std::get<Array>(value);

            std::string str = "";
            str += "[\n";

            for (int i = 0; i < array.size(); i++) {
                str += nextLevelIndentation;
                str += array[i]->toString(level + 1, spaces);

                // append a comma if it is not last element
                if (i != array.size() - 1) str += ",\n";
            }

            str += "\n" + indentation + "]";
            return str;
        }
        case STRING:
            return "\"" + std::get<String>(value) + "\"";
        case NUMBER:
            return std::to_string(std::get<Number>(value));
        case UNSIGNED_NUMBER:
            return std::to_string(std::get<UnsignedNumber>(value));
        case FRACTION:
            return std::to_string(std::get<Fraction>(value));
        case BOOL:
            return std::get<Bool>(value) ? "true" : "false";
        case NULL_:
            return "null";
    }

    return "";
}
