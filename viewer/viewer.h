#ifndef VIEWER_H
#define VIEWER_H

#include <igl/opengl/glfw/Viewer.h>

#include "../model/coral.h"
#include "../model/environment.h"

class CoralViewer : public igl::opengl::glfw::Viewer {
    Coral &coral;
    MarineEnvironment env;
    double time = 0.0;
    bool showEnvironment = true;
    bool isUnderwater = false;

    RowVector3d normalLightColor = RowVector3d(1.0, 1.0, 1.0);
    RowVector3d underwaterLightColor = RowVector3d(0.2, 0.4, 0.6);
    double underwaterFogDensity = 0.1;
    double underwaterLightAttenuationFactor = 0.7;


public:
    explicit CoralViewer(Coral &coral);

    void render();

    bool keyPressed(unsigned int key, int modifier);

    void update(double dt);

    void updateUnderwaterState();

    void applyUnderwaterEffect();

    void toggleUnderwaterView();
};

#endif // VIEWER_H
