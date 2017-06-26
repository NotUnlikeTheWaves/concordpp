#include <iostream>
#include "eventhandler.h"

eventHandler::~eventHandler() {

}

void eventHandler::addRawHandler(std::function<void(nlohmann::json)> callback) {
    std::cout << "add handler zomg" << std::endl;
    callbacks.push_back(callback);
}

void eventHandler::callHandlers(nlohmann::json data) {
    for(auto it = callbacks.begin(); it != callbacks.end(); it++) {
        (*it)(data);
    }
}
