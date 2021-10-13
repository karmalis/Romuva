//
// Created by voltairepunk on 2020-01-23.
//

#ifndef ROMUVA_LUAMODULE_HPP
#define ROMUVA_LUAMODULE_HPP

#include "LuaContext.hpp"
#include "../Module.hpp"

namespace Romuva {

    namespace Scripting {

        using LuaState = lua_State;

        class LuaModule : public Core::Module {
        public:

            enum States {
                INIT,
                POST_INIT,
                RUN
            } state;

            LuaModule();
            ~LuaModule() override;

            bool init() override;
            void postInit() override;
            void update(double delta) override;
            void shutdown() override;

            const LuaContext& getCurrentContext() const;

        protected:

            LuaContext _context;

        };

    }

}


#endif //ROMUVA_LUAMODULE_HPP
