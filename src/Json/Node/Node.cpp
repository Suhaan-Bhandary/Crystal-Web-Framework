#include "Node.h"

#include <stdexcept>

Json::Node::Node() { type = NULL_; }

Json::Node::Node(const Node& otherNode) {
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

Json::Node& Json::Node::operator=(const Node& otherNode) {
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

Json::Node::Node(Node&& otherNode) {
    type = otherNode.type;
    value = otherNode.value;
    otherNode.type = NULL_;
}

Json::Node& Json::Node::operator=(Node&& otherNode) {
    type = otherNode.type;
    value = otherNode.value;
    otherNode.type = NULL_;
    return *this;
}

Json::Node::~Node() { clear(); }

JsonType Json::Node::getType() const { return type; }

// Getters
template <>
const Json::String& Json::Node::get<Json::String>() const {
    return std::get<Json::String>(value);
}

template <>
const Json::Number& Json::Node::get<Json::Number>() const {
    return std::get<Json::Number>(value);
}

template <>
const Json::Fraction& Json::Node::get<Json::Fraction>() const {
    return std::get<Json::Fraction>(value);
}

template <>
const Json::Bool& Json::Node::get<Json::Bool>() const {
    return std::get<Json::Bool>(value);
}

template <>
const Json::Array& Json::Node::get<Json::Array>() const {
    return std::get<Array>(value);
}

template <>
const Json::Object& Json::Node::get<Json::Object>() const {
    return std::get<Object>(value);
}

Json::Node& Json::Node::get(int index) const {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    if (index >= std::get<Array>(value).size()) {
        throw std::invalid_argument("Json Array out of index");
    }

    return *std::get<Array>(value).at(index);
}

Json::Node& Json::Node::get(const String& key) const {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    return *std::get<Object>(value).at(key);
}

// Function to clear the node and set it as null
void Json::Node::clear() {
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
void Json::Node::set(Object value) {
    clear();
    type = OBJECT;
    this->value = value;
}

void Json::Node::set(Array value) {
    clear();
    type = ARRAY;
    this->value = value;
}

void Json::Node::set(String value) {
    clear();
    type = STRING;
    this->value = value;
}

void Json::Node::set(Number value) {
    clear();
    type = NUMBER;
    this->value = value;
}

void Json::Node::set(Fraction value) {
    clear();
    type = FRACTION;
    this->value = value;
}

void Json::Node::set(Bool value) {
    clear();
    type = BOOL;
    this->value = value;
}

void Json::Node::set(const char* value) {
    clear();
    type = STRING;
    this->value = std::string(value);
}

// Object Setters
void Json::Node::set(const std::string& key, Object value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Json::Node::set(const std::string& key, Array value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Json::Node::set(const std::string& key, String value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Json::Node::set(const std::string& key, Number value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Json::Node::set(const std::string& key, Fraction value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Json::Node::set(const std::string& key, Bool value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

void Json::Node::set(const std::string& key, Node* value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    object[key] = value;
}

void Json::Node::set(const std::string& key, const char* value) {
    if (type != OBJECT) {
        throw std::invalid_argument("Json Node not an Object");
    }

    Object& object = std::get<Object>(this->value);
    Node* node = new Node();
    node->set(value);
    object[key] = node;
}

// Push in array
void Json::Node::push(Object value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Json::Node::push(Array value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Json::Node::push(String value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Json::Node::push(Number value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Json::Node::push(Fraction value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Json::Node::push(Bool value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

void Json::Node::push(Node* value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    array.push_back(value);
}

void Json::Node::push(const char* value) {
    if (type != ARRAY) {
        throw std::invalid_argument("Json Node not an Array");
    }

    Array& array = std::get<Array>(this->value);
    Node* node = new Node();
    node->set(value);
    array.push_back(node);
}

// NULL check
bool Json::Node::isNull() { return type == NULL_; }

// String representation
std::string Json::Node::toString(int level, int spaces) {
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
        case FRACTION:
            return std::to_string(std::get<Fraction>(value));
        case BOOL:
            return std::get<Bool>(value) ? "true" : "false";
        case NULL_:
            return "null";
    }

    return "";
}