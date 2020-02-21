//
// Created by voltairepunk on 2020-01-28.
//

#ifndef ROMUVA_CAMERA_HPP
#define ROMUVA_CAMERA_HPP


#include <mathfu/glsl_mappings.h>
#include "../ecs/ECS.hpp"

namespace Romuva {

    namespace Components {

        class Camera : public ECS::Component {
        public:

            Camera(float aspect, float near, float far, float fov);

            ~Camera() override = default;

            mathfu::mat4 getProjectionMatrix();

        protected:

            float _near;
            float _far;
            float _fov;
            float _aspect;

        };

    }

}


#endif //ROMUVA_CAMERA_HPP
