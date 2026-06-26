add_library(engine
    clock.cpp
    events.cpp
    render/render-mesh.cpp
    resources/mesh.cpp
    resources/script.cpp
    resources/shader.cpp
    resources/texture.cpp
    resources/transform.cpp
    scene/components/mesh.cpp
    scene/components/transform.cpp
    scene/entity.cpp
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
    assimp
    scripting_deps
    nlohmann_json::nlohmann_json
    -L/Users/ctheriot/homebrew/Cellar/glfw/3.4/lib -lglfw
    -L/Users/ctheriot/homebrew/Cellar/glm/1.0.3/lib -lglm
    -L/Users/ctheriot/homebrew/Cellar/openal-soft/1.25.2/lib -lopenal
)