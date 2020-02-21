//
// Created by voltairepunk on 2020-01-27.
//

#include "Transform.hpp"
#include "../util/Exceptions.hpp"

namespace Romuva {

    namespace Components {

        Transform::Transform(mathfu::vec3 pos, mathfu::vec3 f, mathfu::vec3 u, mathfu::vec3 r, mathfu::vec3 rot, mathfu::vec3 scale) :
                _position(pos), _forward(f), _up(u), _right(r),
                _rotation(mathfu::Quaternion<float>::FromEulerAngles(rot)), _scale(scale), spacialVelocity{}, angularVelocity{},
                svMutex(), avMutex() {
            recalculateFRU();
        }

        const mathfu::vec3 &Transform::getPosition() const {
            return _position;
        }

        const mathfu::vec3 &Transform::getUp() const {
            return _up;
        }

        const mathfu::vec3 &Transform::getForward() const {
            return _forward;
        }

        const mathfu::vec3 &Transform::getRight() const {
            return _right;
        }

        mathfu::vec3 Transform::getRotation() const {
            return _rotation.ToEulerAngles();
        }

        const mathfu::vec3 &Transform::getDirection(Transform::Direction direction) const {
            switch (direction) {
                case FORWARD:
                    return _forward;
                case RIGHT:
                    return _right;
                case UP:
                    return _up;
            }
            throw BadArgumentException{};
        }

        void Transform::recalculateFRU() {
            const std::lock_guard<std::mutex> angularAccessLock(angularAccessMutex);
            _forward = (_rotation.ToMatrix4() * UNIT_Z).Normalized();
            _right = (_rotation.ToMatrix4() * UNIT_X).Normalized();
            _up = (_rotation.ToMatrix4() * UNIT_Y).Normalized();
        }

        const mathfu::vec3 &Transform::getScale() const {
            return _scale;
        }

        mathfu::mat4 Transform::getModelMatrix() const {
            mathfu::mat4 T = mathfu::mat4::FromTranslationVector(mathfu::vec3{
                -_position.x, -_position.y, -_position.z
            });
            mathfu::mat4 R = _rotation.ToMatrix4();
            mathfu::mat4 S = mathfu::mat4::FromScaleVector(_scale);

            return T * R * S;
        }

        void Transform::clean() {
            Component::clean();
            this->recalculateFRU();
        }

        void Transform::move(mathfu::vec3 vel) {
            const std::lock_guard<std::mutex> lock(svMutex);
            spacialMovement.push_front(vel);
        }

        void Transform::rotateX(float angle) {
            const std::lock_guard<std::mutex> lock(avMutex);
            angularMovement.push_front(std::make_pair<Direction, float>(RIGHT, float(angle)));
        }

        void Transform::rotateY(float angle) {
            const std::lock_guard<std::mutex> lock(avMutex);
            angularMovement.push_front(std::make_pair<Direction, float>(UP, float(angle)));
        }

        void Transform::rotateZ(float angle) {
            const std::lock_guard<std::mutex> lock(avMutex);
            angularMovement.push_front(std::make_pair<Direction, float>(FORWARD, float(angle)));
        }

        // TODO: Re think the implementation
        void Transform::update(double delta) {
            {
                const std::lock_guard<std::mutex> svLock(svMutex);
                {
                    const std::lock_guard<std::mutex> spacialAccessLock(spatialAccessMutex);
                    for (const auto & it : spacialMovement) {
                        _position += it;
                        if (!_dirty) _dirty = true;
                    }
                    spacialMovement.clear();
                }
                const std::lock_guard<std::mutex> avLock(avMutex);
                {
                    const std::lock_guard<std::mutex> angularAccessLock(angularAccessMutex);
                    for (const auto & it : angularMovement) {
                        mathfu::Quaternion<float> q;
                        switch (it.first) {
                            case FORWARD: q = mathfu::Quaternion<float>::FromAngleAxis(it.second * delta, _forward); break;
                            case UP: q = mathfu::Quaternion<float>::FromAngleAxis(it.second * delta, _up); break;
                            case RIGHT: q = mathfu::Quaternion<float>::FromAngleAxis(it.second * delta, _right); break;
                        }
                        _rotation = _rotation * q;
                        if (!_dirty) _dirty = true;
                    }
                    angularMovement.clear();
                }
            }


            Component::update(delta);
        }
    }

}
