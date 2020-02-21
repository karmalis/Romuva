//
// Created by voltairepunk on 2019-12-30.
//

#include "Module.hpp"
#include "Engine.hpp"
#include "log/Logger.hpp"

namespace Romuva {

    namespace Core {

        Core::Module::Module(std::string name, bool repeating, bool background) :
                _name(std::move(name)), _background(background), _repeating(repeating) {

        }

        bool Module::init() {
            gLogDebug << "Initialising module: " << this->_name;
            this->_engine->scheduleCallback<std::function<void(void)>>([this]{
                this->update(0.0);
            }, _repeating, _background);
            return true;
        }

        void Module::update(double) {

        }

        void Module::shutdown() {
            gLogDebug << "Shutting down module: " << this->_name;
        }

        void Module::postInit() {

        }


    }

}
