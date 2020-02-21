//
// Created by voltairepunk on 2019-12-31.
//

#ifndef ROMUVA_MESH_HPP
#define ROMUVA_MESH_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "OpenGLDefinitions.hpp"
#include "Mappings.hpp"

namespace Romuva {

    namespace Video {

        class Mesh {
        public:

            Mesh();
            ~Mesh();

            bool loadMesh(const std::string& path);
            void unload();

            const std::string& getReference();

            void render();

        protected:

            struct Entry {
                Entry(const GLfloat* points, const GLfloat* normals, const GLfloat* texcoords, unsigned int count);
                ~Entry();

                unsigned int count;

                GLuint pointsVBO;
                GLuint colourVBO;

//                GLuint positionBufferId;
//                GLuint normalBufferId;
//                GLuint textureBufferId;

                GLuint vao;
            };

            std::string _reference;
            std::vector<std::unique_ptr<Entry>> _entries;
            
        };

    }

}


#endif //ROMUVA_MESH_HPP
