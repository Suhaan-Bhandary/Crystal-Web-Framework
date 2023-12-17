#include "../Controller/mainController.h"
#include "crystal.h"

// Path registration
void userRouterPaths(http::Router &router) {
    // Register path
    router.get("/", Controller::getHome);
    router.get("/user/:id", Controller::getUser);
    router.get("/user/chat", Controller::getUserChat);
    router.get("/redirect-user", Controller::redirectUserToGoogle);

    // Post route
    router.post("/user", Controller::saveUserData);

    // XO Game
    router.get("/xo-game", Controller::getXOGame);

    // Not Found
    router.get("*", Controller::getNotFound);

    // health
    router.get("/health", Controller::getHealth);
}
