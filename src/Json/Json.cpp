#include "Json.h"

#include "../Logger/Logger.h"

Json::Json::Json(const std::string &body) {
    Logger::log("--------------------------------------");

    Logger::log("Json Data: ");
    Logger::log(body);

    Logger::log("--------------------------------------");
}