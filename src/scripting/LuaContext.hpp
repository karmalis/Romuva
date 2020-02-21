//
// Created by voltairepunk on 2020-01-23.
//

#ifndef ROMUVA_LUACONTEXT_HPP
#define ROMUVA_LUACONTEXT_HPP

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>
#include <functional>
#include <list>
#include "../util/Exceptions.hpp"

typedef int (*lua_CFunction) (lua_State *L);

namespace Romuva {

    namespace Scripting {

        using LuaFunction = int (lua_State *L);

        struct LuaStackSequence {
            int stackCounter;
            lua_State * lstate;

            LuaStackSequence() = delete;
            LuaStackSequence(lua_State * lstate);

            void setGlobal(std::string name);

            void createTable();

            void prepareField(const std::string& name);

            template <typename T>
            void addField(const std::string& name, T&& value);

            bool getFieldOnStack(const std::string& name);
            template <typename T>
            void pushStackFieldValue(T&& value);
            void popStackFromField();
        };



        class LuaContext {
        public:

            LuaContext();
            LuaContext(LuaContext&& lc) noexcept;

            ~LuaContext() noexcept;

            LuaContext(const LuaContext&) = delete;
            LuaContext& operator=(const LuaContext&) = delete;

            LuaContext& operator=(LuaContext&& lc) noexcept;

            bool loadScript(const std::string& path);

            void prepareFunction(const std::string& func);

            template <typename T>
            void prepareArgument(T arg);

            void call(int nargs, int rargs);

            lua_State* getLuaState() const;

        protected:
            lua_State * _lState;
        };

        template <>
        inline void LuaStackSequence::addField(const std::string &key, std::string &&value) {
            lua_pushstring(lstate, key.c_str());
            lua_pushstring(lstate, value.c_str());
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, int &&value) {
            lua_pushstring(lstate, key.c_str());
            lua_pushinteger(lstate, value);
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, unsigned int &&value) {
            lua_pushstring(lstate, key.c_str());
            lua_pushinteger(lstate, value);
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, double &&value) {
            lua_pushstring(lstate, key.c_str());
            lua_pushnumber(lstate, value);
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, float &&value) {
            lua_pushstring(lstate, key.c_str());
            lua_pushnumber(lstate, value);
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, bool &&value) {
            lua_pushstring(lstate, key.c_str());
            lua_pushboolean(lstate, value);
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, LuaFunction &&function) {
            lua_pushstring(lstate, key.c_str());
            lua_pushcfunction(lstate, function);
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, lua_CFunction &&function) {
            lua_pushstring(lstate, key.c_str());
            lua_pushcfunction(lstate, function);
            lua_settable(lstate, -3);
        }

        template <>
        inline void LuaStackSequence::addField(const std::string &key, LuaStackSequence &&table) {
            lua_pushstring(lstate, key.c_str());
            lua_createtable(lstate, 0, 0);
            lua_settable(lstate, -3);
        }

        template<typename T>
        inline void LuaStackSequence::addField(const std::string &key, T &&value) {
            throw NotImplementedException{};
        }

        template <>
        inline void LuaStackSequence::pushStackFieldValue(float &&value) {
            lua_pushnumber(lstate, value);
        }

        template<typename T>
        inline void LuaStackSequence::pushStackFieldValue(T &&value) {
            throw NotImplementedException{};
        }

    }

}


#endif //ROMUVA_LUACONTEXT_HPP
