#include "Saveable.hpp"
#include <string>
class GameManager;
class SaveLoadManager{
     public:
        void save(GameManager g, std::string dir);
};