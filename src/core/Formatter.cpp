#include "core/Formatter.hpp"

string Formatter::moneyString(int value) {
    return "M" + to_string(value);
}

string Formatter::colorString(const Color& color) {
    if (color == Color::DEFAULT) {
        return "DEFAULT";
    } else if (color == Color::BROWN) {
        return "BROWN";
    } else if (color == Color::LIGHTBLUE) {
        return "LIGHT BLUE";
    } else if (color == Color::PINK) {
        return "PINK";
    } else if (color == Color::GRAY) {
        return "GRAY";
    } else if (color == Color::ORANGE) {
        return "ORANGE";
    } else if (color == Color::RED) {
        return "RED";
    } else if (color == Color::YELLOW) {
        return "YELLOW";
    } else if (color == Color::GREEN) {
        return "GREEN";
    } else if (color == Color::DARKBLUE) {
        return "DARK BLUE";
    } else {
        throw ColorSetNotOwnedException();
    }
}

string Formatter::statusString(const PropertyStatus& status) {
    if(status == PropertyStatus::BANK) {
        return "BANK";
    } else if (status == PropertyStatus::MORTGAGED) {
        return "MORTGAGE";
    } else if (status == PropertyStatus::OWNED) {
        return "OWNED";
    }
}

string Formatter::showDiceRoll(string username, const Dice& dice, string position) {
    std::ostringstream oss;
    oss << "Mengocok dadu..." << endl;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << username << " mendarat di : " << position << endl;

    return oss.str();
};

string Formatter::showControlDice(string username, const Dice& dice, string position) {
    std::ostringstream oss;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << username << " mendarat di : " << position << endl;

    return oss.str();
};

string Formatter::makeDeedTable(const LandPlot& landPlot) {
    std::ostringstream oss;
    oss << "+===========================================================+" << endl;
    oss << "|                      AKTA KEPEMILIKAN                     |" << endl;
    oss << "| [" << colorString(landPlot.getColor()) << "] " << landPlot.getName() << " (" << landPlot.getCode() << ") " << " |" << endl;
    oss << "+===========================================================+" << endl;
    oss << "| Harga Beli        : " << moneyString(landPlot.getBuyPrice()) << " |" << endl;
    oss << "| Nilai Gadai       : " << moneyString(landPlot.getMortgageValue()) << " |" << endl;
    oss << "+-----------------------------------------------------------+" << endl;
    oss << "| Sewa (unimproved) : " << moneyString(landPlot.getRentPrice(0)) << " |" << endl;
    oss << "| Sewa (1 rumah)    : " << moneyString(landPlot.getRentPrice(1)) << " |" << endl;
    oss << "| Sewa (2 rumah)    : " << moneyString(landPlot.getRentPrice(2)) << " |" << endl;
    oss << "| Sewa (3 rumah)    : " << moneyString(landPlot.getRentPrice(3)) << " |" << endl;
    oss << "| Sewa (4 rumah)    : " << moneyString(landPlot.getRentPrice(4)) << " |" << endl;
    oss << "| Sewa (hotel)      : " << moneyString(landPlot.getRentPrice(5)) << " |" << endl;
    oss << "+-----------------------------------------------------------+" << endl;
    oss << "| Harga Rumah       : " << moneyString(landPlot.getUpgHousePrice()) << " |" << endl;
    oss << "| Harga Hotel       : " << moneyString(landPlot.getUpgHotelPrice()) << " |" << endl;
    oss << "+===========================================================+" << endl;
    
    PropertyStatus status = landPlot.getPropertyStatus();
    if(status == PropertyStatus::BANK) {
        oss << "| Status : " << statusString(status) << " |" << endl;
    } else {
        // TODO: getOwner() ??
        //oss << "| Status : " << statusString(status) << " (" << landPlot.getOwner() << ") " << " |" << endl;
    }

    oss << "+===========================================================+" << endl;
    
    return oss.str();
}


string showLogger(int turn, string username, string actionType, string detail) {
    return "[" + to_string(turn) + "] " + username + " | " + actionType + " | " + detail + "\n";
};

string Formatter::makeCardList(string name, string description) {
    return "- " + name + "---" + description + "\n";
}

string Formatter::makePlayerList(string name, int money, int totalProperty, int totalCard) {
    std::ostringstream oss;
    oss << "Player " << name << " ----------" << endl;
    oss << "Money    : " << money << endl;
    oss << "Property : " << totalProperty << endl;
    oss << "Cards    : " << totalCard << endl;
    oss << endl;

    return oss.str();
}

string Formatter::makePropertyList(string name, string code) {
    return "- " + code + "(" + name + ")" + "\n";
}

// TODO: Lengkapin yang ini (5)
string Formatter::makePropertyList(Color& color, string code, string name) {
    std::ostringstream oss;
    oss << "[" << colorString(color) << "]" << endl;
    oss << "   - " << name << " (" << code << ")";
}

string Formatter::makeCanMortgagedList(string name, string code, Color& color, int money) {
    std::ostringstream oss;
    oss << name << " (" << code << ") " << "[" << colorString(color) << "] " << "Nilai Gadai : " << moneyString(money) << endl;

    return oss.str();
}

string Formatter::makeMortgagedList(string name, string code, Color& color, int money) {
    std::ostringstream oss;
    oss << name << " (" << code << ") " << "[" << colorString(color) << "] " << "[M] " << "Harga Tebus : " << moneyString(money) << endl;
    
    return oss.str();
}

string Formatter::successMortgage(string name, int moneyRecieved, int moneyTotal) {
    std::ostringstream oss;
    oss << name << "berhasil digadaikan." << endl;
    oss << "Kamu menerima " << moneyString(moneyRecieved) << " dari Bank" << endl;
    oss << "Uang kamu sekarang : " << moneyString(moneyTotal) << endl;
    oss << "Catatan : Sewa tidak dapat dipungut dari properti yang digadaikan" << endl;

    return oss.str();
}

string Formatter::failedMortgage(const LandPlot& landplot) {
    std::ostringstream oss;
    oss << landplot.getName() << " tidak dapat digadaikan!" << endl;
    oss << "Masih terdapat bangunan di color group [" << colorString(landplot.getColor()) << "]." << endl;
    oss << "Bangunan harus dijual terlebih dahulu." << endl;
    oss << endl;
    oss << "Daftar bangunan di color group [" << colorString(landplot.getColor()) << "] :" << endl;
    
    // TODO: Daftar bangunan di warna yang sama
}

string Formatter::makePayRent(const Player& visitor, const Player& owner, int level, int rentValue, string code) {
    std::ostringstream oss;
    oss << "Kamu mendarat di " << code << ", milik " << owner.getUsername() << "!" << endl;
    if (level == 5) {
        oss << "Kondisi : 1 Hotel" << endl; 
    } else if (level > 1)  {
        oss << "Kondisi : " << level << "Rumah" << endl;
    } 

    int visitorMoney = visitor.getTotalWealth();
    int ownerMoney = owner.getTotalWealth();
    oss << "Sewa      : " << moneyString(rentValue) << endl << endl;
    oss << "Uang Kamu : " << moneyString(visitorMoney) << " -> " << visitorMoney - rentValue << endl;
    oss << "Uang Pemain " << owner.getUsername() << " : " <<  ownerMoney + rentValue << endl;

    return oss.str();
}

 

