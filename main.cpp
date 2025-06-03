#include <iostream>

#include "model/coral.h"
#include "viewer/viewer.h"

int main() {
    Coral c;
    for (int i = 0; i < 10; i++) {
        c.grow(.25);
    }
    CoralViewer viewer(c);
    viewer.render();
    viewer.launch();
    return 0;
}
