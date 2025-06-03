#include "viewer.h"

CoralViewer::CoralViewer(Coral &coral): coral(coral) {
    callback_key_pressed = [&](Viewer &, unsigned int key, int modifier) {
        return this->keyPressed(key, modifier);
    };

    callback_pre_draw = [&](Viewer &) -> bool {
        update(0.016);
        return false;
    };
}

void CoralViewer::update(double dt) {
    time += dt;
    env.update(time, dt);
}

void CoralViewer::updateUnderwaterState() {
    Eigen::Vector3f cameraPos = core().camera_eye;
    std::cout << cameraPos << std::endl;
    std::cout << env.waterSurface.level << std::endl;
    isUnderwater = cameraPos[1] < env.waterSurface.level;
}

void CoralViewer::toggleUnderwaterView() {
    isUnderwater = !isUnderwater;
    render();
}

void CoralViewer::render() {
    for (auto &data: data_list) {
        data.clear();
    }

    // floor
    if (showEnvironment) {
        append_mesh();
        data().set_mesh(env.oceanFloor.vertices, env.oceanFloor.faces);
        data().set_colors(env.oceanFloor.color);
        data().show_lines = false;
    }

    // coral
    append_mesh();
    data().set_mesh(coral.vertices, coral.indices);
    data().set_colors(RowVector3d(0.8f, 0.3f, 0.5f));

    // surface
    if (showEnvironment) {
        append_mesh();
        data().set_mesh(env.waterSurface.vertices, env.waterSurface.faces);

        if (isUnderwater) {
            data().set_colors(Eigen::RowVector4d(0.1, 0.3, 0.5, 0.7));
        } else {
            data().set_colors(Eigen::RowVector4d(0.2, 0.5, 0.8, 0.5));
        }

        data().set_face_based(true);
        data().show_lines = false;
    }

    data().shininess = isUnderwater ? 20.0 : 50.0;

    if (isUnderwater) {
        core().background_color = Eigen::RowVector4f(0.2, 0.5, 0.8, 0.5);
    } else {
        core().background_color = Eigen::RowVector4f(0.5, 0.7, 0.9, 0.5);
    }
}

bool CoralViewer::keyPressed(unsigned int key, int modifier) {
    switch (key) {
        case 'g':
        case 'G':
            coral.grow(0.25);
            render();
            return true;
        case 'e':
        case 'E':
            showEnvironment = !showEnvironment;
            render();
            return true;
        case 'u':
        case 'U':
            toggleUnderwaterView();
            return true;
        default:
            return false;
    }
}
