#include "Json.h"

#include <iostream>

#include "Parser/Parser.h"

Crystal::Json::Json::Json() { data = new Node(); }

Crystal::Json::Json::Json(const char* str, bool readFromFile) {
    auto p = Parser(str, readFromFile);
    data = p.parse();
}

Crystal::Json::Json::Json(const Json& otherJson) {
    data = new Node(*otherJson.data);
}

Crystal::Json::Json& Crystal::Json::Json::operator=(const Json& otherJson) {
    if (this != &otherJson) {
        data = new Node(*otherJson.data);
    }
    return *this;
}

Crystal::Json::Json::Json(Json&& otherJson) {
    data = otherJson.data;
    otherJson.data = nullptr;
}

Crystal::Json::Json& Crystal::Json::Json::operator=(
    Crystal::Json::Json&& otherJson) {
    data = otherJson.data;
    otherJson.data = nullptr;
    return *this;
}

Crystal::Json::Json::Json(const Node& otherNode) { data = new Node(otherNode); }

Crystal::Json::Json& Crystal::Json::Json::operator=(const Node& otherNode) {
    data = new Node(otherNode);
    return *this;
}

Crystal::Json::Json::~Json() { delete data; }

Crystal::Json::Node& Crystal::Json::Json::getData() { return *data; }
