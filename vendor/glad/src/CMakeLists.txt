add_library(glad glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/include PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glad/include)
