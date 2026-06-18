add_subdirectory(editor)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/include)
target_link_libraries(${PROJECT_NAME} editor)