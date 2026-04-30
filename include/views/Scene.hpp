#ifndef SCENE_HPP
#define SCENE_HPP

class GameManager;
class AccountManager;
class SceneManager;

class Scene {
public:
    Scene(SceneManager* sm, GameManager* gm, AccountManager* am);
    virtual ~Scene() = default;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void onEnter() {}
protected:
    SceneManager*   sceneManager;
    GameManager*    gameManager;
    AccountManager* accountManager;
};

#endif
