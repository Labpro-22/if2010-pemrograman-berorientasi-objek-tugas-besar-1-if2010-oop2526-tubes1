#include "core/GameEngine.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
#include "sstream"

class Formatter {
    public:
        static string showDiceRoll(string& username, const Dice& dice, string& position);
        static string showControlDice(string& username, const Dice& dice, string& position); 
        static string makeDeedTable(std::map<int, int>);


        static string showLogger(int& turn, string& username, string& actionType, string& detail);
        static string makeCardList(string& name, string& description);
        static string makePlayerList(string& name, int& money, int& totalProperty, int& totalCard);
        static string makePropertyList(string& color, string& name, string& code);
};

