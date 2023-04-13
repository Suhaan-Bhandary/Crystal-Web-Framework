#include "../Controller/Controller.h"
#include "./Router.h"

// Path registration
http::Router::Router() {
    // Initialize the root
    root = new http::PathTrie("ROOT");

    registerPath("GET", "/", Controller::getHome);
    registerPath("GET", "/home", Controller::getHome);
    registerPath("GET", "/user/photos", Controller::getHome);
    registerPath("GET", "/user/:id", Controller::getHome);
    registerPath("GET", "*", Controller::getHome);
}