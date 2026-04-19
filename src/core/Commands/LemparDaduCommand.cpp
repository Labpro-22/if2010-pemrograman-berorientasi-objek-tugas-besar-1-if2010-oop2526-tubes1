#include "LemparDaduCommand.hpp"
#include "GameMaster.hpp"
#include "Player.hpp"
#include "dadu.hpp"
#include <iostream>

LemparDaduCommand::LemparDaduCommand(GameMaster& gm, Player* player, Dice& d)
    : gameMaster(gm), currentPlayer(player), dice(d) {}

void LemparDaduCommand::execute() {
    if (currentPlayer == nullptr) {
        std::cerr << "Error: Tidak ada pemain yang sedang aktif!" << std::endl;
        return;
    }

    std::cout << currentPlayer->getName() << " melempar dadu..." << std::endl;

    // Lakukan pengacakan dadu
    dice.rollRandom();

    int val1 = dice.getDaduVal1();
    int val2 = dice.getDaduVal2();
    int total = dice.getTotal();

    std::cout << "Hasil: Dadu 1 = " << val1 << ", Dadu 2 = " << val2 
              << " (Total: " << total << ")" << std::endl;

    // Cek kondisi double berturut-turut (aturan Monopoli: 3x double masuk penjara)
    if (dice.getConsecutiveDoubles() == 3) {
        std::cout << "Tiga kali double berturut-turut! " << currentPlayer->getName() 
                  << " langsung dijebloskan ke Penjara!" << std::endl;
        
        // Pindahkan pemain ke penjara (Asumsi: ada method sendToJail di GameMaster atau Player)
        gameMaster.sendPlayerToJail(currentPlayer);
        
        // Reset counter double setelah masuk penjara
        dice.resetConsecutiveDouvles(); 
        
        // Akhiri giliran (pemain tidak boleh jalan lagi)
        gameMaster.endCurrentTurn();
    } else {
        if (val1 == val2) {
            std::cout << "DOUBLE! Anda mendapat kesempatan untuk melempar dadu lagi setelah ini." << std::endl;
            // Set flag di GameMaster bahwa pemain ini punya giliran ekstra
            gameMaster.setExtraTurn(true); 
        } else {
            // Jika tidak double, pastikan flag giliran ekstra dimatikan dan counter reset
            gameMaster.setExtraTurn(false);
            dice.resetConsecutiveDouvles();
        }

        // Pindahkan pemain sesuai dengan total dadu
        // Asumsi: gameMaster menangani perpindahan dan efek petak yang diinjak
        gameMaster.movePlayer(currentPlayer, total);
    }
}