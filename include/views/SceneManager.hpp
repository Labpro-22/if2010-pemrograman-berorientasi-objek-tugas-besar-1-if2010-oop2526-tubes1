#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP
#include <memory>

class GameManager;
class AccountManager;
class Scene;
class MainMenuScene;
class InGameScene;

enum class SceneType { MainMenu, InGame };

class SceneManager {
public:
    SceneManager();
    ~SceneManager();
    void initialize(GameManager* gm, AccountManager* am);
    void setScene(SceneType type);
    void update();
    void draw();
private:
    GameManager*    gameManager = nullptr;
    AccountManager* accountManager = nullptr;
    std::unique_ptr<MainMenuScene> mainMenuScene;
    std::unique_ptr<InGameScene>   inGameScene;
    Scene* currentScene = nullptr;
};

#endif
