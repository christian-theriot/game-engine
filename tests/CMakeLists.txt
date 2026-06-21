include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(engine_tests ${TEST_SOURCES})
target_link_libraries(engine_tests 
    PRIVATE
    GTest::gtest_main
    GTest::gmock
    engine
)
target_include_directories(engine_tests PRIVATE ${CMAKE_SOURCE_DIR}/vendor/include ${CMAKE_SOURCE_DIR}/include)

gtest_discover_tests(engine_tests)