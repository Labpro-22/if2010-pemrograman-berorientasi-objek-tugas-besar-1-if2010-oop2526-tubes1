#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP
#include "Scene.hpp"
class InGameScene : public Scene {
public:
    void draw() override;
    void update() override;
};

#endif