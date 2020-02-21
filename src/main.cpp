#include <iostream>

#include "Engine.hpp"

#include "video/VideoModule.hpp"
#include "scripting/LuaModule.hpp"
#include "ecs/Manager.hpp"


int main() {
    Romuva::Core::gEngine.addModule(new Romuva::Scripting::LuaModule);
    Romuva::Core::gEngine.addModule(new Romuva::ECS::Manager);
    Romuva::Core::gEngine.addModule(new Romuva::Video::VideoModule);

    Romuva::Core::gEngine.init();
    Romuva::Core::gEngine.run();
    return 0;
}