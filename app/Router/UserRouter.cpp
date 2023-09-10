#include "../../src/Router/Router.h"
#include "../Controller/mainController.h"

// Path registration
void http::Router::userRouterPaths() {
    // Register path
    registerPath("GET", "/", Controller::getHome);
    registerPath("GET", "/user/:id", Controller::getUser);
    registerPath("GET", "/user/chat", Controller::getUserChat);
    registerPath("GET", "/redirect-user", Controller::redirectUserToGoogle);
    registerPath("POST", "/user", Controller::saveUserData);

    // XO Game
    registerPath("GET", "/xo-game", Controller::getXOGame);

    // Not Found
    registerPath("GET", "*", Controller::getNotFound);

    // health
    registerPath("GET", "/health", Controller::getHealth);
}