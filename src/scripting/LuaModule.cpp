//
// Created by voltairepunk on 2020-01-23.
//

#include "LuaModule.hpp"

#define BOILERPLATE_SCRIPT "scripts/boilerplate.lua"

namespace Romuva {

    namespace Scripting {

        LuaModule::LuaModule() : _context(), state(INIT),
                                 Core::Module("LuaModule", true, true) {}

        bool LuaModule::init() {
            return Module::init();
        }

        void LuaModule::update(double delta) {
            Module::update(delta);

            switch (state) {
                case POST_INIT: {
                    _context.prepareFunction("init");
                    _context.call(0, 0);
                    state = RUN;
                default: {
                    _context.prepareFunction("update");
                    _context.prepareArgument(delta);
                    _context.call(1, 0);
                    }
                }
            }
        }

        void LuaModule::shutdown() {
            Module::shutdown();
        }

        const LuaContext &LuaModule::getCurrentContext() const {
            return _context;
        }

        void LuaModule::postInit() {
            Module::postInit();
            _context.loadScript(BOILERPLATE_SCRIPT);
            state = POST_INIT;
        }

        LuaModule::~LuaModule() = default;
    }
}
