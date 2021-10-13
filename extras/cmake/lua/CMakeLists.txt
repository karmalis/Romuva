set(LUA_LIB_SRCS
        "lapi.c"
        "lcode.c"
        "lctype.c"
        "ldebug.c"
        "ldo.c"
        "ldump.c"
        "lfunc.c"
        "lgc.c"
        "llex.c"
        "lmem.c"
        "lobject.c"
        "lopcodes.c"
        "lparser.c"
        "lstate.c"
        "lstring.c"
        "ltable.c"
        "ltm.c"
        "lundump.c"
        "lvm.c"
        "lzio.c"
        "lauxlib.c"
        "lbaselib.c"
        "lcorolib.c"
        "ldblib.c"
        "liolib.c"
        "lmathlib.c"
        "loadlib.c"
        "loslib.c"
        "lstrlib.c"
        "ltablib.c"
        "lutf8lib.c"
        "linit.c"
        )

if(LUA_BUILD_AS_CXX)
    set_source_files_properties(${LUA_LIB_SRCS} "lua.c" "luac.c" PROPERTIES LANGUAGE CXX )
endif()

add_library(lua_static STATIC ${LUA_LIB_SRCS})
set_target_properties(lua_static PROPERTIES OUTPUT_NAME "lua")
target_include_directories(lua_static PUBLIC "include")
if(UNIX)
    set(LUA_DEFINITIONS)

    if(NOT EMSCRIPTEN)
        find_library(LIBM m)
        #TODO: Redo this with find_package
        if(NOT LIBM)
            message(FATAL_ERROR "libm not found and is required by lua")
        endif()
        target_link_libraries(lua_static INTERFACE ${LIBM})

        list(APPEND LUA_DEFINITIONS LUA_USE_POSIX)
        if(LUA_SUPPORT_DL)
            target_compile_definitions(lua_static PRIVATE "LUA_USE_DLOPEN")
            target_link_libraries(lua_static INTERFACE dl)
        endif()
    endif()

    target_compile_definitions(lua_static
            PUBLIC ${LUA_DEFINITIONS}
            )
    target_compile_options(lua_static
            PRIVATE "-Wall" "-Wextra"
            )
endif()

if(LUA_BUILD_BINARY)
    include(CheckIncludeFile)
    CHECK_INCLUDE_FILE("readline/readline.h" HAVE_READLINE_READLINE_H)


    add_executable(lua "lua.c")
    target_link_libraries(lua PUBLIC lua_static)
    set_target_properties(lua PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
            )
    if (HAVE_READLINE_READLINE_H)
        target_compile_definitions(lua PUBLIC "LUA_USE_READLINE")
        target_link_libraries(lua PUBLIC readline)
    endif()
endif()
if(LUA_BUILD_COMPILER)
    add_executable(luac "luac.c")
    target_link_libraries(luac PUBLIC lua_static)
    set_target_properties(luac PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
            )
endif()
