//
// Created by voltairepunk on 2020-01-28.
//

#include "Camera.hpp"

namespace Romuva {

    namespace Components {


        Camera::Camera(float aspect, float near, float far, float fov) :
                _aspect(aspect), _near(near), _far(far), _fov(fov) {

        }

        mathfu::mat4 Camera::getProjectionMatrix() {
            return mathfu::PerspectiveHelper<float>(_fov, _aspect, _near, _far, 1.0);
        }


    }

}
