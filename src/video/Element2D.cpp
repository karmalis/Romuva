//
// Created by voltairepunk on 2020-01-27.
//

#include "Element2D.hpp"

const float texturePlanePoints[] = {
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f,  1.0f, -1.0f,  1.0f, 1.0f
};

namespace Romuva {

    namespace Video {

        Element2D::Element2D() {
            glGenBuffers(1, &_pointsVBO);
            glBindBuffer(GL_ARRAY_BUFFER, _pointsVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(texturePlanePoints), texturePlanePoints, GL_STATIC_DRAW);

            glGenVertexArrays(1, &_pointsVAO);
            glBindVertexArray(_pointsVAO);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(0);
        }

        Element2D::~Element2D() {
            glDeleteBuffers(1, &_pointsVBO);
            glDeleteBuffers(1, &_pointsVAO);
        }

        void Element2D::render() {
            glDisable(GL_DEPTH_TEST);
            glBindVertexArray(_pointsVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

}
