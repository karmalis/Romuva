//
// Created by voltairepunk on 2019-12-31.
//

#ifndef ROMUVA_SHADER_HPP
#define ROMUVA_SHADER_HPP

#include <string>
#include <map>
#include <vector>

#include "OpenGLDefinitions.hpp"

namespace Romuva {

    namespace Video {

        //using Handler = std::function<void(const tMessage&)>;
        using AttributeMap = std::map<std::string, GLint>;

        class Shader {
        public:

            Shader();
            ~Shader();

            bool loadVertexShader(const std::string& path);
            bool loadFragmentShader(const std::string& path);
            bool linkShaders(std::vector<std::string> attributes = {});

            void enable();
            void disable();

            template <typename T>
            void setUniform(const std::string& name, const T& value);

            bool validateProgram();

            void unload();

        protected:

            std::string loadFile(const std::string& path);
            void reportShaderError(GLuint shader);
            void reportProgramError(GLuint program);

            GLuint _program;

            GLuint _vs;
            GLuint _fs;

            AttributeMap _shaderAttributes;

        };

    }

}


#endif //ROMUVA_SHADER_HPP
