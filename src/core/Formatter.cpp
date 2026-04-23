#include "core/Formatter.hpp"

string Formatter::showDiceRoll(string& username, const Dice& dice, string& position) {
    std::ostringstream oss;
    oss << "Mengocok dadu..." << endl;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << username << " mendarat di " << position << endl;

    return oss.str();
};

string Formatter::showControlDice(string& username, const Dice& dice, string& position) {
    std::ostringstream oss;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << username << " mendarat di " << position << endl;

    return oss.str();
};


string Formatter::makeDeedTable(std::map<int, int>) {

}


string showLogger(int& turn, string& username, string& actionType, string& detail) {
    return "[" + to_string(turn) + "] " + username + " | " + actionType + " | " + detail + "\n";
};

string Formatter::makeCardList(string& name, string& description) {
    return "- " + name + "---" + description + "\n";
}

string Formatter::makePlayerList(string& name, int& money, int& totalProperty, int& totalCard) {
    std::ostringstream oss;
    oss << "Player " << name << " ----------" << endl;
    oss << "Money : " << money << endl;
    oss << "Property : " << totalProperty << endl;
    oss << "Cards : " << totalCard << endl;
    oss << endl;

    return oss.str();
}




