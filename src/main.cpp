#include "../include/core/CommandProcessor.hpp"
#include "../include/views/BoardView.hpp"
#include "../include/views/PropertyView.hpp"

int main() {
    BoardView bv;
    PropertyView pv;

    CommandProcessor cp(nullptr, &bv, &pv);

    while (true) {
        cp.readCommand();
    }

    return 0;
}