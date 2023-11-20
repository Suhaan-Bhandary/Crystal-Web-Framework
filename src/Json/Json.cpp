#include "Json.h"

#include <iostream>

#include "Parser/Parser.h"

Json::Json::Json() { data = new Node(); }

Json::Json::Json(const char* str, bool readFromFile) {
    auto p = Parser();
    data = p.parse(str, readFromFile);
}

Json::Json::Json(const Json& otherJson) { data = new Node(*otherJson.data); }

Json::Json& Json::Json::operator=(const Json& otherJson) {
    if (this != &otherJson) {
        data = new Node(*otherJson.data);
    }
    return *this;
}

Json::Json::Json(Json&& otherJson) {
    data = otherJson.data;
    otherJson.data = nullptr;
}

Json::Json& Json::Json::operator=(Json&& otherJson) {
    data = otherJson.data;
    otherJson.data = nullptr;
    return *this;
}

Json::Json::Json(const Node& otherNode) { data = new Node(otherNode); }

Json::Json& Json::Json::operator=(const Node& otherNode) {
    data = new Node(otherNode);
    return *this;
}

Json::Json::~Json() { delete data; }

Json::Node& Json::Json::getData() { return *data; }
