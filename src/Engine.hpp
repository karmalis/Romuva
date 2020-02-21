//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_ENGINE_HPP
#define ROMUVA_ENGINE_HPP

#include <memory>
#include <vector>
#include <map>

#include "task/TaskManager.hpp"
#include "Module.hpp"


namespace Romuva {

    namespace Core {

        class Module;

        class Engine {
        public:

            using ModulePtr = std::unique_ptr<Module>;
            using ModuleList = std::vector<ModulePtr>;
            using ModuleMap = std::map<std::string, Module*>;

            void addModule(Module* module);
            void removeModule(const std::string& name);
            void removeModule(Module* module);

            Engine();

            bool init();

            void run();

            void shutdown();

            template <typename Func>
            void scheduleCallback(Func f, bool repeating = false, bool background = false) {
                this->_taskManager.addWork(f, repeating, background);
            }

            template <typename T>
            T* getModule() const;

            struct OnStop{};
            void operator()(const OnStop&);

        protected:

            Tasks::TaskManager _taskManager;

            void addModule(std::unique_ptr<Module>&& module);
            bool initialiseModules();
            void shutdownModules();

            ModuleList _modules;
            ModuleMap _moduleLookup;

        };

        template<typename T>
        T *Engine::getModule() const {
            for (const auto& module : _modules) {
                T* result = dynamic_cast<T*>(module.get());
                if (result) {
                    return result;
                }
            }

            return nullptr;
        }

        extern Engine gEngine;

    }

}


#endif //ROMUVA_ENGINE_HPP
