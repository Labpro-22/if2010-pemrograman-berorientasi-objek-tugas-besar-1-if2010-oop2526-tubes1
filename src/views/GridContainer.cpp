#include "../../include/views/GridContainer.hpp"

GridContainer::GridContainer(int rows, int cols) : rows(rows), cols(cols) {}

void GridContainer::add(const GridItem& item) {
    items.push_back(item);
}

void GridContainer::draw() {
    for (auto& item : items) {
        item.draw();
    }
}

void GridContainer::update() {
    for (auto& item : items) {
        item.update();
    }
}

void GridContainer::layout() {
    float cellWidth = boundingBox.width / cols;
    float cellHeight = boundingBox.height / rows;

    for (auto& item : items) {
        int row = item.row;
        int col = item.col;
        int rowSpan = item.rowSpan;
        int colSpan = item.colSpan;

        Rectangle itemBound;
        itemBound.x = boundingBox.x + col * cellWidth;
        itemBound.y = boundingBox.y + row * cellHeight;
        itemBound.width = cellWidth * colSpan;
        itemBound.height = cellHeight * rowSpan;

        item.setBoundary(itemBound);
    }
}