//
// Created by voltairepunk on 2020-01-03.
//

#ifndef ROMUVA_CAMERA1_HPP
#define ROMUVA_CAMERA1_HPP

#include <mathfu/glsl_mappings.h>

namespace Romuva {

    namespace Video {



        class Camera {
        public:

            enum Direction {
                FORWARD, RIGHT, UP
            };

            Camera(float aspect, float near, float far, float fov, const mathfu::vec3 &position,
                   const mathfu::vec3 &rotation);

            ~Camera();

            mathfu::mat4 getProjectionMatrix();

            mathfu::mat4 getViewMatrix();

            void rotateX(float angleR);

            void rotateY(float angleR);

            void rotateZ(float angleR);

            void setPosition(const mathfu::vec3& position);

            void move(const float& amount, const Direction& direction);

            const mathfu::vec3 &getPosition();

            mathfu::vec3 getRotation();

            const mathfu::vec3& getDirection(Direction direction);

        protected:

            void recalculateFRU();

            float _near;
            float _far;
            float _fov;
            float _aspect;

            mathfu::vec3 _forward;
            mathfu::vec3 _right;
            mathfu::vec3 _up;

            mathfu::vec3 _position;
            mathfu::Quaternion<float> _rotation;
        };

    }

}


#endif //ROMUVA_CAMERA_HPP
