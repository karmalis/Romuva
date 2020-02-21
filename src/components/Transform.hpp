//
// Created by voltairepunk on 2020-01-27.
//

#ifndef ROMUVA_TRANSFORM_HPP
#define ROMUVA_TRANSFORM_HPP

#include <forward_list>
#include <mathfu/glsl_mappings.h>
#include "../ecs/ECS.hpp"

static const mathfu::vec3 UNIT_Z{0.0f, 0.0f, -1.0f};
static const mathfu::vec3 UNIT_Y{0.0f, 1.0f, 0.0f};
static const mathfu::vec3 UNIT_X{1.0f, 0.0f, 0.0f};

namespace Romuva {

    namespace Components {

    class Transform : public ECS::Component {
    public:

        enum Direction {
            FORWARD, RIGHT, UP
        };

        Transform(mathfu::vec3 pos, mathfu::vec3 f, mathfu::vec3 u, mathfu::vec3 r, mathfu::vec3 rot, mathfu::vec3 scale);
        ~Transform() override = default;

        void clean() override;
        void update(double delta) override;

        const mathfu::vec3& getPosition() const;
        const mathfu::vec3& getUp() const;
        const mathfu::vec3& getForward() const;
        const mathfu::vec3& getRight() const;
        const mathfu::vec3& getScale() const;
        mathfu::vec3 getRotation() const;

        mathfu::mat4 getModelMatrix() const;

        const mathfu::vec3& getDirection(Direction direction) const;

        void move(mathfu::vec3 vel);
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);

    protected:
        mathfu::vec3 _position;
        mathfu::Quaternion<float> _rotation;
        mathfu::vec3 _scale;

        mathfu::vec3 _up;
        mathfu::vec3 _forward;
        mathfu::vec3 _right;

        mutable mathfu::vec3 spacialVelocity;
        mutable mathfu::Quaternion<float> angularVelocity;

        std::forward_list<mathfu::vec3> spacialMovement;
        std::forward_list<std::pair<Direction, float>> angularMovement;

        std::mutex svMutex;
        std::mutex avMutex;
        std::mutex spatialAccessMutex;
        std::mutex angularAccessMutex;

        void recalculateFRU();
    };

    }

}

#endif //ROMUVA_TRANSFORM_HPP
