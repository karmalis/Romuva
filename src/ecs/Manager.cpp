//
// Created by voltairepunk on 2019-12-30.
//
#include <algorithm>

#include "Manager.hpp"
#include "../Engine.hpp"
#include "../scripting/LuaModule.hpp"

namespace Romuva {

    namespace ECS {

        Manager::Manager() : Core::Module("ECSManager", true, true) {

        }

        bool Manager::init() {
            return  Core::Module::init();
        }

        void Manager::update(double delta) {
            for (auto &e : this->_entities) e->update(1.0);
            this->refresh();
        }

        void Manager::shutdown() {
            for (auto &e : this->_entities) e->destroy();
            Core::Module::shutdown();
        }

        void Manager::draw() {
            for (auto &e : this->_entities) e->draw();
        }

        void Manager::refresh() {
            for (auto i(0u); i < maxGroups; i++) {
                auto &v(this->_groupedEntities[i]);
                v.erase(
                        std::remove_if(std::begin(v), std::end(v), [i](Entity *entity) {
                            return !entity->isActive() || !entity->hasGroup(i);
                        }), std::end(v));
            }

            this->_entities.erase(std::remove_if(std::begin(this->_entities), std::end(this->_entities),
                                                 [](const std::unique_ptr<Entity> &entity) {
                                                     return !entity->isActive();
                                                 }), std::end(this->_entities));

        }

        void Manager::addToGroup(Entity *entity, Group group) {
            this->_groupedEntities[group].emplace_back(entity);
        }

        std::vector<Entity *> &Manager::getGroup(Group group) {
            return this->_groupedEntities[group];
        }

        Entity &Manager::addEntity() {
            auto* e = new Entity(*this);
            std::unique_ptr<Entity> uPtr{e};
            this->_entities.emplace_back(std::move(uPtr));

            return *e;
        }





        int entityCreate(Scripting::LuaState* lstate) {

//            auto ** udata = (Entity **) lua_newuserdata(lstate, sizeof(Entity *));
//            //udata =

            return 0;
        }



        void Manager::postInit() {
            Module::postInit();

//            auto* luaModule = _engine->getModule<Scripting::LuaModule>();
//
//            Scripting::LuaStackSequence ecsLibrary(luaModule->getCurrentContext().getLuaState());
//
//            ecsLibrary.pushStackFieldValue(42.f);
//            ecsLibrary.setGlobal("ECS");
//
//            //            ecsLibrary.createTable();
////            ecsLibrary.prepareField("Manager");
////            ecsLibrary.createTable();
////            ecsLibrary.prepareField("Entity");
////            ecsLibrary.createTable();
////            ecsLibrary.addField("Create", &createEntity);

        }


    }

}