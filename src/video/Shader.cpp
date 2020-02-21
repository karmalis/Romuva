//
// Created by voltairepunk on 2019-12-31.
//

#include "Shader.hpp"
#include "../log/Logger.hpp"

#include <fstream>
#include <mathfu/glsl_mappings.h>

namespace Romuva {

    namespace Video {


        Shader::Shader() : _program(0), _vs(0), _fs(0) {
            _program = glCreateProgram();
        }

        Shader::~Shader() {
            unload();
        }

        bool Shader::loadVertexShader(const std::string &path) {
            _vs = glCreateShader(GL_VERTEX_SHADER);

            std::string vsContents = loadFile(path);
            GLint status = GL_FALSE;
            const char * vsContentsPtr = vsContents.c_str();
            GLint contentSize = vsContents.size();

            glShaderSource(_vs, 1, &vsContentsPtr, &contentSize);
            glCompileShader(_vs);
            glGetShaderiv(_vs, GL_COMPILE_STATUS, &status);

            if (status == GL_FALSE) {
                gLogError << "Could not compile vertex shader: " << glGetError() << " " << path;
                reportShaderError(_vs);
                return false;
            }

            glAttachShader(_program, _vs);

            return true;
        }

        bool Shader::loadFragmentShader(const std::string &path) {
            _fs = glCreateShader(GL_FRAGMENT_SHADER);

            std::string vsContents = loadFile(path);
            GLint status = GL_FALSE;
            const char * vsContentsPtr = vsContents.c_str();
            GLint contentSize = vsContents.size();

            glShaderSource(_fs, 1, &vsContentsPtr, &contentSize);
            glCompileShader(_fs);
            glGetShaderiv(_fs, GL_COMPILE_STATUS, &status);

            if (status == GL_FALSE) {
                gLogError << "Could not compile fragment shader: " << glGetError() << " " << path;
                reportShaderError(_fs);
                return false;
            }

            glAttachShader(_program, _fs);

            return true;
        }

        bool Shader::linkShaders(std::vector<std::string> attributes) {
            glLinkProgram(_program);

            GLint status = GL_FALSE;
            glGetProgramiv(_program, GL_LINK_STATUS, &status);
            if (status == GL_FALSE) {
                gLogError << "Could not link program ";
                reportProgramError(_program);
                return false;
            }

            for (const auto& attribute : attributes) {
                GLint location = glGetUniformLocation(_program, attribute.c_str());
                if (location > -1) {
                    this->_shaderAttributes[attribute] = location;
                }
            }

            return validateProgram();
        }

        void Shader::enable() {
            glUseProgram(_program);
        }

        void Shader::disable() {
        }

        void Shader::unload() {
            glDetachShader(_program, _vs);
            glDetachShader(_program, _fs);

            glDeleteShader(_vs);
            glDeleteShader(_fs);

            glDeleteProgram(_program);
        }

        bool Shader::validateProgram() {
            glValidateProgram(_program);
            GLint params = -1;
            glGetProgramiv(_program, GL_VALIDATE_STATUS, &params);
            if (GL_TRUE != params) {
                reportProgramError(_program);
                return false;
            }
            return true;
        }

        std::string Shader::loadFile(const std::string &path) {
            std::string contents;
            std::ifstream ifs(path);

            if (ifs) {
                ifs.seekg(0, std::ios::end);
                contents.resize(ifs.tellg());
                ifs.seekg(0, std::ios::beg);
                ifs.read(&contents[0], contents.size());
                ifs.close();
            }

            return contents;
        }

        void Shader::reportShaderError(GLuint shader) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::string errorLog;
            errorLog.resize(maxLength);

            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            gLogError << errorLog;
        }

        void Shader::reportProgramError(GLuint program) {
            int actualLength = 0;
            char log[2048];

            glGetProgramInfoLog(program, 2048, &actualLength, log);

            gLogError << std::string(log);
        }

        template<typename T>
        void Shader::setUniform(const std::string& name, const T& value) {
            throw std::exception{};
        }

        template<>
        void Shader::setUniform(const std::string& name, const mathfu::vec4& value) {
            if (this->_shaderAttributes.find(name) != this->_shaderAttributes.end()) {
                glUniform4f(this->_shaderAttributes[name], value.x, value.y, value.z, value.w);
            }
        }

        template<>
        void Shader::setUniform(const std::string& name, const mathfu::mat4& value) {
            if (this->_shaderAttributes.find(name) != this->_shaderAttributes.end()) {
                glUniformMatrix4fv(this->_shaderAttributes[name], 1, GL_FALSE, &value[0]);
            }
        }

        template<>
        void Shader::setUniform(const std::string &name, const mathfu::vec2& value) {
            if (this->_shaderAttributes.find(name) != this->_shaderAttributes.end()) {
                glUniform2f(this->_shaderAttributes[name], value.x, value.y);
            }
        }

    }

}
