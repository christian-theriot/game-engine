add_subdirectory(glad)

include(FetchContent)
FetchContent_Declare(
    JoltPhysics
    GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics.git
    GIT_TAG v5.3.0
    SOURCE_SUBDIR Build
)
FetchContent_MakeAvailable(JoltPhysics)

FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/lua/lua.git
    GIT_TAG v5.4.7
)
FetchContent_MakeAvailable(lua)

file(GLOB LUA_SOURCES "${lua_SOURCE_DIR}/*.c")
list(FILTER LUA_SOURCES EXCLUDE REGEX ".*/(lua|luac)\\.c$")

add_library(lua_lib STATIC ${LUA_SOURCES})
target_include_directories(lua_lib PUBLIC ${lua_SOURCE_DIR})

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(sol2)

FetchContent_Declare(
    wasmtime
    URL https://github.com/bytecodealliance/wasmtime/releases/download/v25.0.0/wasmtime-v25.0.0-aarch64-macos-c-api.tar.xz
)
FetchContent_MakeAvailable(wasmtime)

add_library(scripting_deps INTERFACE)
target_include_directories(scripting_deps INTERFACE
    ${wasmtime_SOURCE_DIR}/include
)
target_link_libraries(scripting_deps INTERFACE
    sol2::sol2
    lua_lib
    ${wasmtime_SOURCE_DIR}/lib/libwasmtime.a
)
target_compile_definitions(scripting_deps INTERFACE SOL_USE_STD_OPTIONAL=1 SOL_USE_BOOST=0)
target_compile_features(scripting_deps INTERFACE cxx_std_17)