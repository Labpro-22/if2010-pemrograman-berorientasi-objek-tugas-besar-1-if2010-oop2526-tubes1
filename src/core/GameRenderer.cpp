#include "core/GameRenderer.hpp"

void GameRenderer::showDiceRoll(string username, const Dice& dice, string position) {
    cout << "Mengocok dadu..." << endl;
    showControlDice(username, dice, position);
};

void GameRenderer::showControlDice(string username, const Dice& dice, string position) {
    cout << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    cout << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    cout << username << " mendarat di " << position << endl;
};

void GameRenderer::showDeed(PropertyPlot& property) {
    cout << "+===========================================+" << endl;
    cout << "|              AKTA KEPEMILIKAN             |" << endl;
    cout << "| " << property.getName() << " (" << property.getCode() << ")" << " |" << endl;
    cout << "+===========================================+" << endl;
    
};    
void GameRenderer::showPropertyBrief(std::vector<std::reference_wrapper<Plot>>& proprties) {

};



void GameRenderer::showSaveSuccess(string filename) {
    cout << "Menyimpan permainan..." << endl;
    cout << "Permainan berhasil disimpan ke: " << filename << endl;
};

void GameRenderer::showSaveFailed(string filename) {
    cout << "Gagal menyimpan file! pastikan direktori dapat ditulis." << endl;
}

void GameRenderer::showLogger(const LogEntry& entry){
    cout << "[" << entry.turn << "] " << entry.username << " | " << entry.actionType << " | " << entry.detail << endl; 
}

void GameRenderer::showBoard(const GameState& state) {
    cout << BoardFormatter::boardFormat(
        state.getBoard(),
        state.getPlayers(),
        state.getCurrentTurn(),
        state.getMaxTurn()
    );
};