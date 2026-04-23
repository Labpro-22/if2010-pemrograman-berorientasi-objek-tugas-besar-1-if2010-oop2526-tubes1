#include "core/GameEngine.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "sstream"

class Formatter {
    public:
        static string moneyString(int value);
        static string colorString(const Color& color);
        static string statusString(const PropertyStatus& status);

        static string showDiceRoll(string username, const Dice& dice, string position);
        static string showControlDice(string username, const Dice& dice, string position); 
        static string makeDeedTable(const LandPlot& landPlot);


        static string showLogger(int turn, string username, string actionType, string detail);
        static string makeCardList(string name, string description);
        static string makePlayerList(string name, int money, int totalProperty, int totalCard);
        
        static string makePropertyList(string name, string code);
        static string makePropertyList(Color& color, string name, string code); // TODO:

        static string makeMortgagedList(string name, string code, Color& color, int money);
        static string makeCanMortgagedList(string name, string code, Color& color, int money);
        static string successMortgage(string name, int moneyRecieved, int moneyTotal);
        static string failedMortgage(const LandPlot& landplot); //TODO:
        

        static string makePayRent(const Player& visitor, const Player& owner, int level, int rentValue, string code); 



};

