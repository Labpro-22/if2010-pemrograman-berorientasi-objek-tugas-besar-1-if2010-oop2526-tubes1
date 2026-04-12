#ifndef GRID_CONTAINER_HPP
#define GRID_CONTAINER_HPP
#include <vector>
#include "UIElement.hpp"
#include "GridItem.hpp"

class GridContainer : public UIElement {
public:
    GridContainer(int rows, int cols);
    void add(const GridItem& item);
    void draw() override;
    void update() override;
    void layout();

private:
    int rows;
    int cols;
    std::vector<GridItem> items;
};

#endif