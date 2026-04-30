#ifndef NIMONSPOLI_HPP
#define NIMONSPOLI_HPP
#include "SceneManager.hpp"
#include "../core/GameManager.hpp"
#include "../core/AccountManager.hpp"

class Nimonspoli {
public:
    void setup();
    void start();
private:
    GameManager    gameManager;
    AccountManager accountManager;
    SceneManager   sceneManager;
    void update();
    void draw();
};

#endif
