#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

#include <vector>
#include <string>
#include "Button.hpp"
#include "Scene.hpp"
#include "TextField.hpp"

class MainMenuScene : public Scene {
public:
    MainMenuScene(SceneManager* sm, GameManager* gm, AccountManager* am);
    void update() override;
    void draw()   override;
    void onEnter() override;
private:
    // Config folder
    TextField configPathField;
    // Player setup
    Button newGameButton;
    Button quitButton;
    Button startGameButton;
    Button cancelButton;
    Button plusButton;
    Button minusButton;

    std::vector<TextField> playerFields;
    bool  showSetupModal;
    TextField configPathField;
    std::string formError;
    bool  showNewGameModal;
    int   playerCount;
    float sceneTime;
    float modalVis;
    std::string errorMsg;

    void layoutButtons(Rectangle sr);
    void drawBackground(Rectangle sr);
    void drawHero(Rectangle sr);
    void drawFeatureCards(Rectangle sr);
    void drawSetupModal(Rectangle sr);
    void onStartGame();
};

#endif
