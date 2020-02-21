//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_VIDEOMODULE_HPP
#define ROMUVA_VIDEOMODULE_HPP

#include "../Module.hpp"

#include "OpenGLDefinitions.hpp"

namespace Romuva {

    namespace Video {

        class VideoModule : public Core::Module {
        public:

            VideoModule();
            ~VideoModule() override;

            bool init() override;
            void postInit()override;
            void update(double delta) override;
            void shutdown() override;

            static void errorCallback(int error, const char * description);

        protected:

            GLFWwindow* _window;
            bool _keepRendering;

            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void characterCallback(GLFWwindow* window, unsigned int codepoint);
            static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
            static void onCursorEnterCallback(GLFWwindow* window, int entered);
            static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        };


    }

}

#endif //ROMUVA_VIDEOMODULE_HPP
