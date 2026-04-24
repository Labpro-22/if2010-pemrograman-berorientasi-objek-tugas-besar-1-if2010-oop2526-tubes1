#include "../include/utils/IOManager.hpp"
#include "../include/utils/ConfigData.hpp"

// untuk test doang 
int main(){
    FullConfigData config = IOManager::loadAllConfigs("./config");

    for(auto& tile : config.streets){
        std::cout << tile->getIndex() <<" "<< tile->getName()<<" "<< tile->getCode()<<" "<< tile->getColor() << std::endl;
    }
    std::cout << config.

    return 0;
}