#include "../../include/views/GridItem.hpp"

GridItem::GridItem(UIElement* item, int row, int col, int rowSpan, int colSpan) 
    : item(item), row(row), col(col), rowSpan(rowSpan), colSpan(colSpan) {}

void GridItem::draw() {
    if (item) {
        item->draw();
    }
}

void GridItem::update() {
    if (item) {
        item->update();
    }
}

void GridItem::layout() {
    if (item) {
        item->setBoundary(boundingBox);
    }
}