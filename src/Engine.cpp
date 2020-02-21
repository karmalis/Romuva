//
// Created by voltairepunk on 2019-12-30.
//

#include <algorithm>

#include "Engine.hpp"
#include "messaging/Channel.hpp"
#include "log/Logger.hpp"

namespace Romuva {

    namespace Core {

        Engine::Engine() {
            Messaging::Channel::add<OnStop>(this);
        }

        bool Engine::init() {
            gLogDebug << "Initialising Engine";
            return this->initialiseModules();
        }

        void Engine::run() {
            gLogDebug << "Running Engine";
            this->_taskManager.start();
        }

        void Engine::shutdown() {
            gLogDebug << "Shutting down Engine";
            this->_taskManager.stop();
            this->shutdownModules();
        }

        void Engine::addModule(Module *module) {
            gLogDebug << "Adding module: " << module->_name;
            this->addModule(std::unique_ptr<Module>(module));
        }

        void Engine::removeModule(const std::string& name) {
            gLogDebug << "Removing module: " << name;

            Module* m = nullptr;

            auto it = this->_moduleLookup.find(name);
            if (it != this->_moduleLookup.end()) {
                m = it->second;
            }

            if (m) this->removeModule(m);
            else {
                gLogError << "Module " << name << " is not loaded.";
            }
        }

        void Engine::removeModule(Module *module) {
            auto it = std::find_if(
                    this->_modules.begin(),
                    this->_modules.end(),
                    [module](const ModulePtr& ptr) {
                        return ptr.get() == module;
                    }
            );

            if (it != this->_modules.end()) {
                module->shutdown();
                this->_modules.erase(it);

                for (auto jt = this->_moduleLookup.begin(); jt != this->_moduleLookup.end(); ++jt) {
                    if (jt->second == module) {
                        this->_moduleLookup.erase(jt);
                        break;
                    }
                }
            }

        }

        void Engine::operator()(const Engine::OnStop &) {
            this->shutdown();
        }

        void Engine::addModule(std::unique_ptr<Module> &&module) {
            auto it = this->_moduleLookup.find(module->getName());

            if (it == this->_moduleLookup.end()) {
                module->_engine = this;
                this->_moduleLookup[module->getName()] = module.get();
                this->_modules.emplace_back(std::move(module));
            } else {
               gLogWarning << "Module already added, skipping: " << module->getName();
            }
        }

        bool Engine::initialiseModules() {
            gLogDebug << "Initialising modules";
            for (const auto& module : this->_modules) {
                if (!module->init()) {
                    gLogFatal << "Could not initialize module: " << module->getName();
                    return false;
                }
            }

            gLogDebug << "Running post initialization steps";
            for (const auto& module : this->_modules) {
                module->postInit();
            }

            return true;

        }

        void Engine::shutdownModules() {
            gLogDebug << "Shutting down modules";
            for (const auto& module : this->_modules) {
                module->shutdown();
            }

            this->_modules.clear();
            this->_moduleLookup.clear();
        }

        Engine gEngine;
    }

}
