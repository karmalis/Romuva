//
// Created by voltairepunk on 2020-01-03.
//

#include "Camera.hpp"

#include "../log/Logger.hpp"

const mathfu::vec3 UNIT_Z{0.0f, 0.0f, -1.0f};
const mathfu::vec3 UNIT_Y{0.0f, 1.0f, 0.0f};
const mathfu::vec3 UNIT_X{1.0f, 0.0f, 0.0f};

namespace Romuva {

    namespace Video {


        Camera::Camera(float aspect, float near, float far, float fov, const mathfu::vec3 &position,
                       const mathfu::vec3 &rotation) : _near(near), _far(far), _fov(fov), _aspect(aspect),
                                                       _position(position),
                                                       _rotation(mathfu::Quaternion<float>::FromEulerAngles(rotation)) {
            recalculateFRU();
        }

        Camera::~Camera() {

        }

        void Camera::recalculateFRU() {
            _forward = (_rotation.ToMatrix4() * UNIT_Z).Normalized();
            _right = (_rotation.ToMatrix4() * UNIT_X).Normalized();
            _up = (_rotation.ToMatrix4() * UNIT_Y).Normalized();
        }

        mathfu::mat4 Camera::getProjectionMatrix() {
            return mathfu::PerspectiveHelper<float>(_fov, _aspect, _near, _far, 1.0);
        }

        mathfu::mat4 Camera::getViewMatrix() {
            mathfu::mat4 T = mathfu::mat4::FromTranslationVector(mathfu::vec3{
                    -_position.x, -_position.y, -_position.z
            });
            mathfu::mat4 R = _rotation.ToMatrix4();
            return R * T;
        }

        void Camera::rotateX(float angleR) {
            mathfu::Quaternion<float> q = mathfu::Quaternion<float>::FromAngleAxis(angleR, _right);
            this->_rotation = this->_rotation * q;
            recalculateFRU();
        }

        void Camera::rotateY(float angleR) {
            mathfu::Quaternion<float> q = mathfu::Quaternion<float>::FromAngleAxis(angleR, _up);
            this->_rotation = this->_rotation * q;
            recalculateFRU();
        }

        void Camera::rotateZ(float angleR) {
            mathfu::Quaternion<float> q = mathfu::Quaternion<float>::FromAngleAxis(angleR, _forward);
            this->_rotation = this->_rotation * q;
            recalculateFRU();
        }

        void Camera::setPosition(const mathfu::vec3& position) {
            _position =  position;
        }

        void Camera::move(const float& amount, const Direction& direction) {
            mathfu::vec3 last = _position;
            _position += (amount * getDirection(direction));
            mathfu::vec3 movedBy = _position - last;
        }

        const mathfu::vec3 &Camera::getPosition() {
            return _position;
        }

        mathfu::vec3 Camera::getRotation() {
            return _rotation.ToEulerAngles();
        }

        const mathfu::vec3 &Camera::getDirection(Camera::Direction direction) {
            switch (direction) {
                case FORWARD: return _forward;
                case RIGHT: return _right;
                case UP: return _up;
            }
            throw std::exception{};
        }
    }

}
