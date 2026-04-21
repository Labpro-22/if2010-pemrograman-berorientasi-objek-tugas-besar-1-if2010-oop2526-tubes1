#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include <string>

class GameController;
class BoardView;
class PropertyView;

class CommandProcessor {
private:
    GameController* gameController;
    BoardView* boardView;
    PropertyView* propertyView;

public:
    CommandProcessor(GameController* gc, BoardView* bv, PropertyView* pv);
    void readCommand();
    void executeCommand(const std::string& cmd);
};

#endif