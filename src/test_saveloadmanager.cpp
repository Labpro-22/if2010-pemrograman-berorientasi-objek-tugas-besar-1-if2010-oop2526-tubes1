#include "../include/core/SaveLoadManager.hpp"
#include <fstream>
#include <iostream>

void createTestSaveFile(const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to create test file: " << filename << std::endl;
    return;
  }

  std::cout << "Creating test save file: " << filename << std::endl;

  file << "5 20\n";

  file << "2\n";

  file << "Alice 2000 5 ACTIVE\n";
  file << "2\n";
  file << "MoveCard 5\n";
  file << "ShieldCard\n";

  file << "Bob 1500 10 ACTIVE\n";
  file << "1\n";
  file << "DiscountCard 30 2\n";

  file << "Alice Bob\n";

  file << "Alice\n";

  file << "3\n";

  file << "ST1 street Alice OWNED 1 0 0\n";

  file << "ST2 street Bob MORTGAGED 1 0 0\n";

  file << "RR1 railroad BANK BANK 1 0 0\n";

  file << "2\n";
  file << "MoveCard\n";
  file << "DiscountCard\n";

  file << "2\n";
  file << "1 Alice ROLL_DICE Lempar dadu: 3+2=5\n";
  file << "2 Bob BUY_PROPERTY Membeli ST2\n";

  file.close();
  std::cout << "Test save file created successfully." << std::endl;
}

void verifyAndPrintFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open file: " << filename << std::endl;
    return;
  }

  std::cout << "\n=== FILE CONTENT ===" << std::endl;
  std::string line;
  int lineNum = 1;
  while (std::getline(file, line)) {
    std::cout << "Line " << lineNum << ": " << line << std::endl;
    lineNum++;
  }
  file.close();
}

void testFileExists() {
  std::cout << "\n=== TEST: existFile() ===" << std::endl;

  std::string testFile = "data/test_save_1.sav";
  bool exists = SaveLoadManager::existFile(testFile);
  std::cout << "File '" << testFile << "' exists: " << (exists ? "YES" : "NO")
            << std::endl;

  createTestSaveFile(testFile);

  exists = SaveLoadManager::existFile(testFile);
  std::cout << "After creating file, exists: " << (exists ? "YES" : "NO")
            << std::endl;

  if (exists) {
    verifyAndPrintFile(testFile);
  }
}

void testFileFormat() {
  std::cout << "\n=== TEST: File Format Parsing ===" << std::endl;

  std::string testFile = "data/test_save_format.sav";

  std::ofstream file(testFile);
  file << "5 20\n";
  file << "2\n";
  file << "Player1 1000 0 ACTIVE\n";
  file << "0\n";
  file << "Player2 900 5 ACTIVE\n";
  file << "0\n";
  file << "Player1 Player2\n";
  file << "Player1\n";
  file << "1\n";
  file << "PROP1 street Player1 OWNED 1 0 2\n";
  file << "0\n";
  file << "0\n";
  file.close();

  std::cout << "Test format file created." << std::endl;

  std::ifstream testInput(testFile);
  if (!testInput.is_open()) {
    std::cerr << "Failed to open test file" << std::endl;
    return;
  }

  int turn, maxTurn;
  testInput >> turn >> maxTurn;
  std::cout << "Turn: " << turn << " / " << maxTurn << std::endl;

  int numPlayers;
  testInput >> numPlayers;
  std::cout << "Number of players: " << numPlayers << std::endl;

  std::string username;
  int money, position;
  std::string status;

  for (int i = 0; i < numPlayers; i++) {
    testInput >> username >> money >> position >> status;
    std::cout << "  Player " << (i + 1) << ": " << username << " (M" << money
              << ") at position " << position << " [" << status << "]"
              << std::endl;

    int cardCount;
    testInput >> cardCount;
    std::cout << "    Has " << cardCount << " cards" << std::endl;
  }

  std::string line;
  std::getline(testInput, line);
  std::getline(testInput, line);
  std::cout << "Turn order: " << line << std::endl;

  std::getline(testInput, line);
  std::cout << "Active player: " << line << std::endl;

  testInput.close();
  std::cout << "Format parsing successful!" << std::endl;
}

int main() {
  std::cout << "=== SaveLoadManager Test Driver ===" << std::endl;

  system("mkdir -p data");

  testFileExists();
  testFileFormat();

  std::cout << "\n=== All tests completed ===" << std::endl;
  std::cout << "\nNOTE: Full save/load tests require GameEngine implementation."
            << std::endl;
  std::cout << "Current tests verify file format and I/O operations."
            << std::endl;

  return 0;
}
