add_library(glad glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} main.cpp version.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include /Users/ctheriot/homebrew/Cellar/glfw/3.4/include /Users/ctheriot/homebrew/Cellar/glm/1.0.3/include)
target_link_libraries(${PROJECT_NAME} glad -L/Users/ctheriot/homebrew/Cellar/glfw/3.4/lib -lglfw -L/Users/ctheriot/homebrew/Cellar/glm/1.0.3/lib -lglm)