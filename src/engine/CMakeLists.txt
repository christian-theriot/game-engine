add_library(engine
    camera.cpp
    clock.cpp
    image.cpp
    material.cpp
    mesh.cpp
    version.cpp
    window.cpp
    primitives/line.cpp
    primitives/cube.cpp
    primitives/plane.cpp
    primitives/sphere.cpp
    resource-manager.cpp
    transform.cpp
    entity.cpp
    world.cpp
    components/mesh.cpp
    components/transform.cpp
    components/rigidbody.cpp
    systems/render.cpp
    systems/transform.cpp
    systems/physics.cpp
    events.cpp
    input.cpp
    audio-manager.cpp
    scripting.cpp
    bindings.cpp
    systems/scripting.cpp
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
    -L/Users/ctheriot/homebrew/Cellar/glfw/3.4/lib -lglfw
    -L/Users/ctheriot/homebrew/Cellar/glm/1.0.3/lib -lglm
    -L/Users/ctheriot/homebrew/Cellar/openal-soft/1.25.2/lib -lopenal
)