//
// Created by voltairepunk on 2020-01-02.
//

#ifndef ROMUVA_MAPPINGS_HPP
#define ROMUVA_MAPPINGS_HPP

#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>
#include <mathfu/matrix.h>

namespace Romuva {

    using Vector2f = mathfu::Vector<float, 2>;
    using Vector3f = mathfu::Vector<float, 3>;
    using Vector4f = mathfu::Vector<float, 4>;

    using Matrix3f = mathfu::mat3;
    using Matrix4f = mathfu::mat4;
    using Quaternionf = mathfu::Quaternion<float>;

    struct Vertex2f {
        Vector2f position;
        Vector2f textureCoordinate;
        Vector2f normal;
    };

    struct Vertex3f {
        Vector3f position;
        Vector2f texturecoord;
        Vector3f normal;
        Vector3f tangent;
    };

    struct VertexPos {
        Vertex3f pos;
    };


}

#endif //ROMUVA_MAPPINGS_HPP
