//
// Created by voltairepunk on 2019-12-31.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "../util/Random.hpp"
#include "../log/Logger.hpp"

namespace Romuva {

    namespace Video {

        Mesh::Mesh() : _reference(Util::Random::generate<std::string>(64)) {}

        Mesh::~Mesh() {
            unload();
        }

        bool Mesh::loadMesh(const std::string &path) {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);

            if (!scene) {
                gLogError << "Error importing file: " << path << " => " << importer.GetErrorString();
                return false;
            }

            bool hasTangents = false, hasNormals = false;

            for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
                if (scene->mMeshes[i]->HasNormals() && !hasNormals) {
                    hasNormals = true;
                }
                if (scene->mMeshes[i]->HasTangentsAndBitangents() && !hasTangents) {
                    hasTangents = true;
                }
                if (hasNormals && hasTangents) {
                    break;
                }
            }

            if (hasNormals) {
                scene = importer.ApplyPostProcessing(aiPostProcessSteps::aiProcess_GenNormals);
            }

            if (hasTangents) {
                scene = importer.ApplyPostProcessing(aiPostProcessSteps::aiProcess_CalcTangentSpace);
            }

            for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
                const aiMesh *mesh = scene->mMeshes[i];

                auto *points = new GLfloat[mesh->mNumVertices * 3];
                auto *colors = new GLfloat[mesh->mNumVertices * 3];
                auto *normals = new GLfloat[mesh->mNumVertices * 3];
                auto *texcoords = new GLfloat[mesh->mNumVertices * 2];

//                GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
//
//                GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

                for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                    const aiVector3D &pos = mesh->mVertices[j];
                    aiColor4D* colour = mesh->mColors[j];
                    const aiVector3D &normal = mesh->mNormals[j];

                    points[j * 3] = (GLfloat) pos.x;
                    points[j * 3 + 1] = (GLfloat) pos.y;
                    points[j * 3 + 2] = (GLfloat) pos.z;

                    colors[j * 3] = (GLfloat) normal.x;
                    colors[j * 3 + 1] = (GLfloat) normal.y;
                    colors[j * 3 + 2] = (GLfloat) normal.z;

                    normals[j * 3] = (GLfloat) normal.x;
                    normals[j * 3 + 1] = (GLfloat) normal.y;
                    normals[j * 3 + 2] = (GLfloat) normal.z;

                    if (mesh->HasTextureCoords(0)) {
                        texcoords[j * 2] = (GLfloat) mesh->mTextureCoords[0][j].x;
                        texcoords[j * 2 + 1] = (GLfloat) mesh->mTextureCoords[0][j].y;
                    }
                }

                this->_entries.emplace_back(std::make_unique<Entry>(points, colors, nullptr, mesh->mNumVertices));

                delete[] points;
                delete[] normals;
                delete[] texcoords;
            }

            return true;
        }

        void Mesh::unload() {
            this->_entries.clear();
        }

        const std::string &Mesh::getReference() {
            return _reference;
        }

        void Mesh::render() {
            glEnable( GL_DEPTH_TEST );
            glBindVertexArray(this->_entries[0]->vao);
            //gLogError << this->_entries[0].vao << this->_entries[0].positionBufferId << this->_entries[0].normalBufferId;
            //glBindVertexArray(2);
            glDrawArrays(GL_TRIANGLES, 0, (this->_entries[0]->count));
//            for (const auto& entry : this->_entries) {
//                glBindVertexArray(entry.vao);
//                glDrawArrays(GL_TRIANGLES, 0, 3);
//            }

            /* OTHER STUFF GOES HERE NEXT */
//            GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
//
//            GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
//
//            GLuint points_vbo;
//            glGenBuffers( 1, &points_vbo );
//            glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
//            glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( GLfloat ), points, GL_STATIC_DRAW );
//
//            /* create a second VBO, containing the array of colours.
//            note that we could also put them both into a single vertex buffer. in this
//            case we would use the pointer and stride parameters of glVertexAttribPointer()
//            to describe the different data layouts */
//            GLuint colours_vbo;
//            glGenBuffers( 1, &colours_vbo );
//            glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );
//            glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( GLfloat ), colours, GL_STATIC_DRAW );
//
//            /* create the VAO.
//            we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
//            the memory should be fetched for vertex shader input variables 0, and 1,
//            respectively. we also have to explicitly enable both 'attribute' variables.
//            'attribute' is the older name for vertex shader 'in' variables. */
//            GLuint vao;
//            glGenVertexArrays( 1, &vao );
//            glBindVertexArray( vao );
//            glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
//            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//            glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );
//            glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//            glEnableVertexAttribArray( 0 );
//            glEnableVertexAttribArray( 1 );
//
//            glBindVertexArray(vao);
//            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        Mesh::Entry::Entry(const GLfloat *points, const GLfloat *normals, const GLfloat *texcoords, unsigned int count)
                : count(count) {
//            if (points != nullptr) {
//                glGenBuffers(1, &positionBufferId);
//                glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
//                glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
//
////                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
////                glEnableVertexAttribArray(0);
//            }
//
//            if (normals != nullptr) {
//                glGenBuffers(1, &normalBufferId);
//                glBindBuffer(GL_ARRAY_BUFFER, normalBufferId);
//                glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
//
////                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
////                glEnableVertexAttribArray(1);
//            }
//
//            if (texcoords != nullptr) {
//                glGenBuffers(1, &textureBufferId);
//                glBindBuffer(GL_ARRAY_BUFFER, textureBufferId);
//                glBufferData(GL_ARRAY_BUFFER, 2 * count, texcoords, GL_STATIC_DRAW);
//
////                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
////                glEnableVertexAttribArray(2);
//            }


            glGenBuffers( 1, &pointsVBO );
            glBindBuffer( GL_ARRAY_BUFFER, pointsVBO );
            glBufferData( GL_ARRAY_BUFFER, count * 3 * sizeof( GLfloat ), points, GL_STATIC_DRAW );

            /* create a second VBO, containing the array of colours.
            note that we could also put them both into a single vertex buffer. in this
            case we would use the pointer and stride parameters of glVertexAttribPointer()
            to describe the different data layouts */
            glGenBuffers( 1, &colourVBO );
            glBindBuffer( GL_ARRAY_BUFFER, colourVBO );
            glBufferData( GL_ARRAY_BUFFER, count * 3 * sizeof( GLfloat ), normals, GL_STATIC_DRAW );

            /* create the VAO.
            we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
            the memory should be fetched for vertex shader input variables 0, and 1,
            respectively. we also have to explicitly enable both 'attribute' variables.
            'attribute' is the older name for vertex shader 'in' variables. */
            glGenVertexArrays( 1, &vao );
            glBindVertexArray( vao );
            glBindBuffer( GL_ARRAY_BUFFER, pointsVBO );
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
            glBindBuffer( GL_ARRAY_BUFFER, colourVBO );
            glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 0 );
            glEnableVertexAttribArray( 1 );

        }

        Mesh::Entry::~Entry() {
            glDeleteBuffers(1, &pointsVBO);
            glDeleteBuffers(1, &colourVBO);
//            glDeleteBuffers(1, &textureBufferId);
            glDeleteBuffers(1, &vao);
        }
    }

}
