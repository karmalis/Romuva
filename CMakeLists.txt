cmake_minimum_required(VERSION 3.15)
project(romuva)

set(CMAKE_CXX_STANDARD 14)

#GLFW
set(OpenGL_GL_PREFERENCE GLVND)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/vendor/glfw)

add_subdirectory(${GLFW_DIR})

# GLAD
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/vendor/glad)
add_library("glad" ${GLAD_DIR}/build/src/glad.c src/util/Random.cpp src/util/Random.hpp src/input/Input.cpp src/input/Input.hpp src/util/Exceptions.hpp)

# ASSIMP
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_STATIC_LIB ON)


# INCLUDES
include_directories(
        "${GLFW_DIR}/include"
        "${GLAD_DIR}/build/include"
        "${CMAKE_SOURCE_DIR}/vendor/assimp/include"
        "${CMAKE_SOURCE_DIR}/vendor/lua/src"
        "${CMAKE_SOURCE_DIR}/vendor/mathfu/include"
        "${CMAKE_SOURCE_DIR}/vendor/mathfu/dependencies/vectorial/include"
        "${CMAKE_SOURCE_DIR}/vendor/stb"
)


add_subdirectory(vendor/assimp)
add_subdirectory(vendor/lua)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(romuva
        src/main.cpp
        src/task/Task.cpp
        src/task/TaskManager.cpp
        src/Module.cpp
        src/Engine.cpp
        src/ecs/Manager.cpp
        src/ecs/ECS.cpp
        src/video/VideoModule.cpp
        src/Active.cpp
        src/log/LogLevel.cpp
        src/log/LogMessage.cpp
        src/log/LogMessage.hpp src/log/LogSink.cpp src/log/LogSink.hpp src/log/Logger.cpp src/log/Logger.hpp src/video/Shader.cpp src/video/Shader.hpp src/video/Mesh.cpp src/video/Mesh.hpp src/video/Camera.cpp src/video/Camera.hpp src/scripting/LuaModule.cpp src/scripting/LuaModule.hpp src/scripting/LuaContext.cpp src/scripting/LuaContext.hpp src/video/Element2D.cpp src/video/Element2D.hpp src/task/ConcurrentPriorityQueue.cpp src/task/ConcurrentPriorityQueue.hpp src/components/Transform.cpp src/components/Transform.hpp src/components/Camera.cpp src/components/Camera.hpp)

target_link_libraries(romuva Threads::Threads liblua assimp glfw ${GLFW_LIBRARIES} glad)
target_include_directories(romuva PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(romuva PRIVATE "GLFW_INCLUDE_NONE")

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
