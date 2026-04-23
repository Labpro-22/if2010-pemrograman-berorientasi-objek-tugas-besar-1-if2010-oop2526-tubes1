#include "iostream"
#include "core/GameEngine.hpp"
#include "core/Formatter.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
using namespace std;

class GameRenderer {
    public:
        static void showDiceRoll(string username, const Dice& dice, string position); // 2
        static void showControlDice(string username, const Dice& dice, string position); // 3

        static void showDeed(PropertyPlot& property);
        




        static void showPropertyBrief(std::vector<std::reference_wrapper<Plot>>&);


        static void showPropertyDetailed(std::vector<std::reference_wrapper<Plot>>&); // 5.1
        static void showBuyProperty(std::vector<std::reference_wrapper<Plot>>&); // 5.2


        static void showRentPayment();

        static void showFestival(); // 13
        static void showSaveSuccess(string filename); // 15.1
        // 15.3 TODO: Kalau nama file sudah ada
        static void showSaveFailed(string filename); // 15.2


        static void showLogger(const LogEntry& entry); //17
        static void showBoard(const GameState& state);
};