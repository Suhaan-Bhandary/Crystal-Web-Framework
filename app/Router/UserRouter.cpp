#include "../../src/Router/Router.h"
#include "../Controller/mainController.h"

// Path registration
http::Router::Router() {
    // Initialize the root
    root = new http::PathTrie("ROOT", 0);

    // Register public files
    registerPublicPath();

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
}