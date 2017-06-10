#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <vector>
#include <functional>
#include <json.hpp>

class eventHandler {
public:
    // eventHandler();
    ~eventHandler();
    void addRawHandler(std::function<void(nlohmann::json)> callback);
    void callHandlers(nlohmann::json data);
private:
    std::vector<std::function<void(nlohmann::json)>> callbacks;
};
#endif
