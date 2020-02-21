//
// Created by voltairepunk on 2020-01-23.
//

#include "LuaContext.hpp"

#include "../log/Logger.hpp"
#include "../util/Exceptions.hpp"

namespace Romuva {

    namespace Scripting {

        LuaContext::LuaContext() {
            _lState = luaL_newstate();

            if (_lState == nullptr) {
                gLogFatal << "Could not create LUA context";
                throw std::bad_alloc();
            }

            lua_atpanic(_lState, [](lua_State* state) -> int {
                const std::string str = lua_tostring(state, -1);
                lua_pop(state, 1);
                gLogError << "LUA PANIC: " << str;
                return 0;
            });

            luaL_openlibs(_lState);
        }

        LuaContext::~LuaContext() noexcept {
            if (_lState) {
                lua_close(_lState);
            }
        }

        LuaContext::LuaContext(LuaContext &&lc) noexcept : _lState(lc._lState)  {
            lc._lState = luaL_newstate();
            lua_atpanic(lc._lState, [](lua_State* state) -> int {
                const std::string str = lua_tostring(state, -1);
                lua_pop(state, 1);
                gLogError << "LUA PANIC: " << str;
                return 0;
            });

            luaL_openlibs(lc._lState);
        }

        LuaContext &LuaContext::operator=(LuaContext &&lc) noexcept {
            std::swap(_lState, lc._lState);
            return *this;
        }

        bool LuaContext::loadScript(const std::string& path) {
            if (luaL_loadfile(_lState, path.c_str())) {
                gLogError << "Could not load script: " << path;
                return false;
            }

            if (lua_pcall(_lState, 0, 0, 0)) {
                gLogError << "Could not run script: " << path;
                return false;
            }

            return false;
        }

        void LuaContext::prepareFunction(const std::string& func) {
            lua_getglobal(_lState, func.c_str());
        }

        template<typename T>
        void LuaContext::prepareArgument(T) {
            throw NotImplementedException{};
        }

        void LuaContext::call(int nargs, int rargs) {
            if (lua_pcall(_lState, nargs, rargs, 0)) {
                gLogError << "Could not call lua function";
            }
        }

        lua_State *LuaContext::getLuaState() const {
            return _lState;
        }

        template<>
        void LuaContext::prepareArgument(int arg) {
            lua_pushinteger(_lState, arg);
        }

        template<>
        void LuaContext::prepareArgument(float arg) {
            lua_pushnumber(_lState, arg);
        }

        template<>
        void LuaContext::prepareArgument(double arg) {
            lua_pushnumber(_lState, arg);
        }

        template<>
        void LuaContext::prepareArgument(bool arg) {
            lua_pushboolean(_lState, arg);
        }

        template<>
        void LuaContext::prepareArgument(std::string arg) {
            lua_pushstring(_lState, arg.c_str());
        }

//        bool LuaContext::call(const std::string &function, const std::list<Value> &args, int nres) {
//            lua_getglobal(_lState, function.c_str());
//
//
//            for (const auto& value : args) {
//                switch (value.type) {
//                    case LuaContext::Value::STRING: {
//                        lua_pushstring(_lState, std::string(value.getValue()).c_str());
//                        break;
//                    }
//                    case LuaContext::Value::NUMBER: {
//                        break;
//                    }
//                    case LuaContext::Value::BOOLEAN: {
//                        break;
//                    }
//
//                }
//            }
//
//            return false;
//        }


        LuaStackSequence::LuaStackSequence(lua_State * lstate) : lstate(lstate), stackCounter(0) {

        }

        bool LuaStackSequence::getFieldOnStack(const std::string &name) {
            if (lstate == nullptr) return false;
            lua_getfield(lstate, -1, name.c_str());
            stackCounter--;
            return true;
        }

        void LuaStackSequence::popStackFromField() {
            if (lstate == nullptr) return;
            lua_pop(lstate, -1);
            stackCounter++;
        }

        void LuaStackSequence::createTable() {
            lua_createtable(lstate, 0, 0);
            stackCounter--;
        }

        void LuaStackSequence::prepareField(const std::string &name) {
            lua_pushstring(lstate, name.c_str());
            stackCounter--;
        }

        void LuaStackSequence::setGlobal(std::string name) {
            lua_setglobal(lstate, name.c_str());
            stackCounter--;
        }


    }
}

