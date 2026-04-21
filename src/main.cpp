#include "../include/core/DataManager.hpp"
#include "../include/core/GameManager.hpp"

#include <exception>
#include <iostream>

int main() {

    GameManager& gm = GameManager::getInstance();
    std::string defaultConfigPath = "config/";
    DataManager dm(
        defaultConfigPath + "misc.txt",
        defaultConfigPath + "property.txt",
        defaultConfigPath + "tax.txt",
        defaultConfigPath + "utility.txt",
        defaultConfigPath + "railroad.txt"
    );

}
