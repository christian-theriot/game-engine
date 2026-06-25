add_library(engine
    clock.cpp
    events.cpp
    resources/mesh.cpp
    resources/shader.cpp
    resources/texture.cpp
    version.cpp
    window.cpp
)
target_include_directories(engine PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    /Users/ctheriot/homebrew/Cellar/glfw/3.4/include
    /Users/ctheriot/homebrew/Cellar/glm/1.0.3/include
    /Users/ctheriot/homebrew/Cellar/openal-soft/1.25.2/include
)
target_compile_features(engine PUBLIC cxx_std_17)
target_link_libraries(engine
    glad
    Jolt
    scripting_deps
    nlohmann_json::nlohmann_json
    -L/Users/ctheriot/homebrew/Cellar/glfw/3.4/lib -lglfw
    -L/Users/ctheriot/homebrew/Cellar/glm/1.0.3/lib -lglm
    -L/Users/ctheriot/homebrew/Cellar/openal-soft/1.25.2/lib -lopenal
)