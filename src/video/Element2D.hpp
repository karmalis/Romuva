//
// Created by voltairepunk on 2020-01-27.
//

#ifndef ROMUVA_ELEMENT2D_HPP
#define ROMUVA_ELEMENT2D_HPP

#include "OpenGLDefinitions.hpp"

namespace Romuva {
    namespace Video {

        class Element2D {
        public:

            Element2D();
            ~Element2D();

            void render();

        protected:

            GLuint _pointsVBO;
            GLuint _pointsVAO;

        };

    }
}



#endif //ROMUVA_ELEMENT2D_HPP
