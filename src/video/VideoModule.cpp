//
// Created by voltairepunk on 2019-12-30.
//

#include "VideoModule.hpp"
#include "../messaging/Channel.hpp"
#include "../Engine.hpp"
#include "../log/Logger.hpp"

#include <mathfu/glsl_mappings.h>

#include "../input/Input.hpp"

#define STB_IMAGE_IMPLEMENTATION

/* TODO Possibly temporary  */
#include <cstdio>
#include <stb_image.h>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Element2D.hpp"
#include "../ecs/Manager.hpp"
#include "../components/Transform.hpp"
#include "../components/Camera.hpp"

/* TODO Possibly temporary ends */
#include "../scripting/LuaModule.hpp"

void _update_fps_counter(GLFWwindow *window) {
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        double fps = (double) frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

Romuva::Video::Mesh *testMesh = nullptr;


bool load_texture(const char * file_name, GLuint *tex) {
    int x, y, n;
    int force_channels = 4;
    unsigned char * image_data = stbi_load(file_name, &x, &y, &n, force_channels);
    if (!image_data) {
        gLogError << "Could not load image: " << file_name;
        return false;
    }

    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        gLogWarning << "Image is not power of 2 dimmensions " << file_name;
    }

//    int width_in_bytes = x * 4;
//    unsigned char * top = nullptr;
//    unsigned char * bottom = nullptr;
//    unsigned char temp = 0;
//    int half_height = y / 2;
//
//    for (int row = 0; row < half_height; row++) {
//        top = image_data + row * width_in_bytes;
//        bottom = image_data + (y - row - 1) * width_in_bytes;
//        for (int col = 0; col < width_in_bytes; col++) {
//            temp = *top;
//            *top = *bottom;
//            *bottom = temp;
//            top++;
//            bottom++;
//        }
//    }

    glGenTextures(1, tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    GLfloat max_aniso = 0.0f;
    glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso );
    // set the maximum!
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso );
    return true;
}

namespace Romuva {

    namespace Video {

        Shader *test;
        Shader *shader2D;
        GLuint gui_tex;
        Element2D* guiElement;
        ECS::Entity* testEntity;

        VideoModule::VideoModule() : Core::Module("VideoModule", true, false),
                                     _window(nullptr), _keepRendering(false) {
        }

        bool VideoModule::init() {
            gLogDebug << "Initialising GLFW";

            glfwSetErrorCallback(VideoModule::errorCallback);

            if (!glfwInit()) {
                gLogFatal << "Could not initialise GLFW";
                return false;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwWindowHint(GLFW_SAMPLES, 16);

            gLogDebug << "Creating a window";

            this->_window = glfwCreateWindow(1280, 720, "Romuva", nullptr, nullptr);
            if (!this->_window) {
                gLogFatal << "Could not create window";
                glfwTerminate();
                return false;
            }

            glfwMakeContextCurrent(this->_window);

            gLogDebug << "Initialising GLAD";
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                gLogFatal << "Could not load extra OpenGL features";
                glfwDestroyWindow(this->_window);
                glfwTerminate();
                return false;
            }

            glfwSwapInterval(1);

            glfwSetCharCallback(_window, VideoModule::characterCallback);
            glfwSetKeyCallback(_window, VideoModule::keyCallback);
            glfwSetCursorPosCallback(_window, VideoModule::cursorPositionCallback);
            glfwSetCursorEnterCallback(_window, VideoModule::onCursorEnterCallback);
            glfwSetMouseButtonCallback(_window, VideoModule::mouseButtonCallback);

            // get version info
            const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
            const GLubyte *version = glGetString(GL_VERSION); // version as a string
            printf("Renderer: %s\n", renderer);
            printf("OpenGL version supported %s\n", version);

//            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_CULL_FACE);    // cull face
            glCullFace(GL_BACK);         // cull back face
            glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front
            //glClearColor( 0.2, 0.2, 0.2, 1.0 ); // grey background to help spot mistakes

            // TODO: TEMPORARY START
//            glGenBuffers(1, &vbo);
//            glBindBuffer(GL_ARRAY_BUFFER, vbo);
//            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
//
//            glGenVertexArrays(1, &vao);
//            glBindVertexArray(vao);
//            glEnableVertexAttribArray(0);
//            glBindBuffer(GL_ARRAY_BUFFER, vbo);
//            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

            test = new Shader();
            test->loadVertexShader("data/shaders/basic_vs.glsl");
            test->loadFragmentShader("data/shaders/basic_fs.glsl");
            test->linkShaders({"view", "proj", "model"});

            shader2D = new Shader();
            shader2D->loadVertexShader("data/shaders/2d_vs.glsl");
            shader2D->loadFragmentShader("data/shaders/2d_fs.glsl");
            shader2D->linkShaders({"gui_scale"});

            testMesh = new Mesh();
            testMesh->loadMesh("data/models/monkey.obj");

            testEntity = &this->_engine->getModule<ECS::Manager>()->addEntity();
            testEntity->addComponent<Components::Transform>(
                    mathfu::vec3{0,0,2.f},
                    UNIT_Z, UNIT_Y, UNIT_X,
                    mathfu::vec3{0,0,0},
                    mathfu::vec3{1.f, 1.f, 1.f}
                    );
            testEntity->addComponent<Components::Camera>((1270 / 720), 0.1, 100.0, 70.f);


            guiElement = new Element2D();

            load_texture("data/textures/test1.png", &gui_tex);

            // TODO: TEMPORARY END

            _keepRendering = true;

            return Module::init();
        }

        float last_position = 0.0;
        float speed = 1.0f;

        void VideoModule::update(double delta) {
            Module::update(delta);

            if (glfwWindowShouldClose(this->_window)) {
                Messaging::Channel::broadcast(Core::Engine::OnStop());
                _keepRendering = false;
            }


            if (_keepRendering) {

                if (fabs(last_position) > 1.0f) {
                    speed = -speed;
                }

                // TODO: PART OF TEMPORARRY START
                int width, height;

                glfwGetFramebufferSize(this->_window, &width, &height);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glViewport(0, 0, width, height);

                auto& cameraComponent = testEntity->getComponent<Components::Camera>();
                auto& transformComponent = testEntity->getComponent<Components::Transform>();

                test->enable();
                test->setUniform("model", mathfu::mat4::FromTranslationVector(mathfu::vec3{0, 0, -2.f}));
                test->setUniform("view", transformComponent.getModelMatrix());
                test->setUniform("proj", cameraComponent.getProjectionMatrix());
                testMesh->render();
                test->disable();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, gui_tex);
                shader2D->enable();
                shader2D->setUniform("gui_scale", mathfu::vec2{32.f / 1270, 32.f / 720});
                guiElement->render();
                shader2D->disable();



                // TODO: PART OF TEMPORARRY END
                glfwSwapBuffers(_window);
                glfwPollEvents();
                //std::this_thread::sleep_for(std::chrono::milliseconds(300));
                _update_fps_counter(this->_window);

                if (glfwGetKey(_window, GLFW_KEY_LEFT)) {
                    transformComponent.rotateY(-0.009);
                }
                if (glfwGetKey(_window, GLFW_KEY_RIGHT)) {
                    transformComponent.rotateY(0.009);
                }
                if (glfwGetKey(_window, GLFW_KEY_UP)) {
                    transformComponent.rotateX(0.009);
                }
                if (glfwGetKey(_window, GLFW_KEY_DOWN)) {
                    transformComponent.rotateX(-0.009);
                }
                if (glfwGetKey(_window, GLFW_KEY_E)) {
                    transformComponent.rotateZ(0.009);
                }
                if (glfwGetKey(_window, GLFW_KEY_Q)) {
                    transformComponent.rotateZ(-0.009);
                }
                if (glfwGetKey(_window, GLFW_KEY_W)) {
                    transformComponent.move((0.025f * UNIT_Z));
                }
                if (glfwGetKey(_window, GLFW_KEY_S)) {
                    transformComponent.move((-0.025f * UNIT_Z));
                }
                if (glfwGetKey(_window, GLFW_KEY_A)) {
                    transformComponent.move((0.025f * UNIT_X));
                }
                if (glfwGetKey(_window, GLFW_KEY_D)) {
                    transformComponent.move((-0.025f * UNIT_X));
                }
                if (glfwGetKey(_window, GLFW_KEY_Z)) {
                    transformComponent.move((0.025f * UNIT_Y));
                }
                if (glfwGetKey(_window, GLFW_KEY_X)) {
                    transformComponent.move((-0.025f * UNIT_Y));
                }


            }
        }

        void VideoModule::shutdown() {
            delete guiElement;
            delete testMesh;
            test->unload();
            delete test;

            gLogDebug << "Destroying window";
            if (_window) {
                glfwDestroyWindow(_window);
            }
            gLogDebug << "Terminating GLFW";
            glfwTerminate();

            Module::shutdown();
        }

        void VideoModule::errorCallback(int error, const char *description) {
            gLogError << "GLFW Error: code " << error << " msg: " << description;
        }

        void VideoModule::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
            Messaging::Channel::broadcast(Input::OnKeyAction{key, scancode, action, mods});
        }

        void VideoModule::characterCallback(GLFWwindow *window, unsigned int codepoint) {
            Messaging::Channel::broadcast(Input::OnCharAction{codepoint});
        }

        void VideoModule::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
            Messaging::Channel::broadcast(Input::OnMouseMove{xpos, ypos});
        }

        void VideoModule::onCursorEnterCallback(GLFWwindow *window, int entered) {
            Messaging::Channel::broadcast(Input::OnMouseEnter{entered != 0});
        }

        void VideoModule::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
            Messaging::Channel::broadcast(Input::OnMouseButtonAction{button, action, mods});
        }

        void VideoModule::postInit() {
            Module::postInit();

            auto* luaModule = this->_engine->getModule<Scripting::LuaModule>();

        }

        VideoModule::~VideoModule() = default;


    }

}
