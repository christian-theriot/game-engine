add_subdirectory(engine/v1)
add_subdirectory(engine/v2)

add_executable(${PROJECT_NAME} engine/v2/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/include)
target_link_libraries(${PROJECT_NAME} engine)

add_executable(${PROJECT_NAME}-v1 engine/v1/main-v1.cpp)
target_include_directories(${PROJECT_NAME}-v1 PRIVATE ${CMAKE_SOURCE_DIR}/vendor/include)
target_link_libraries(${PROJECT_NAME}-v1 engine-v1)