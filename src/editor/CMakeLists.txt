add_library(editor
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
    scene.cpp
)
target_include_directories(editor PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    /Users/ctheriot/homebrew/Cellar/glfw/3.4/include
    /Users/ctheriot/homebrew/Cellar/glm/1.0.3/include
)
target_link_libraries(editor
    glad 
    -L/Users/ctheriot/homebrew/Cellar/glfw/3.4/lib -lglfw
    -L/Users/ctheriot/homebrew/Cellar/glm/1.0.3/lib -lglm
)