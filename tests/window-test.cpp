#include <engine/window.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockGLFWWindowImpl : public Engine::IWindowImpl<GLFWwindow *>
{
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(void, terminate, (), (override));
    MOCK_METHOD(GLFWwindow *, createWindow, (int width, int height, const char *title), (override));
    MOCK_METHOD(void, makeContextCurrent, (GLFWwindow * window), (override));
    MOCK_METHOD(bool, loadGlad, (), (override));
};

TEST(WindowTest, FailsWhenGlfwInitFails)
{
    auto mockImpl = std::make_unique<MockGLFWWindowImpl>();
    auto *mock = mockImpl.get();

    EXPECT_CALL(*mock, init()).WillOnce(::testing::Return(false));
    EXPECT_CALL(*mock, createWindow(::testing::_, ::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(*mock, terminate());

    Engine::Window window(std::move(mockImpl));

    EXPECT_FALSE(window.isInitialized());
}

TEST(WindowTest, FailsWhenWindowCreationFails)
{
    auto mockImpl = std::make_unique<MockGLFWWindowImpl>();
    auto *mock = mockImpl.get();

    EXPECT_CALL(*mock, init()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mock, createWindow(::testing::_, ::testing::_, ::testing::_)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mock, makeContextCurrent(::testing::_)).Times(0);
    EXPECT_CALL(*mock, terminate());

    Engine::Window window(std::move(mockImpl));

    EXPECT_FALSE(window.isInitialized());
}

TEST(WindowTest, FailsWhenGladFails)
{
    auto mockImpl = std::make_unique<MockGLFWWindowImpl>();
    auto *mock = mockImpl.get();
    auto *fakeWindow = reinterpret_cast<GLFWwindow *>(0xDEAD);

    EXPECT_CALL(*mock, init()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mock, createWindow(::testing::_, ::testing::_, ::testing::_)).WillOnce(::testing::Return(fakeWindow));
    EXPECT_CALL(*mock, makeContextCurrent(fakeWindow));
    EXPECT_CALL(*mock, loadGlad()).WillOnce(::testing::Return(false));
    EXPECT_CALL(*mock, terminate());

    Engine::Window window(std::move(mockImpl));

    EXPECT_TRUE(window.isInitialized()); // window != nullptr even if GLAD failed
}

TEST(WindowTest, SucceedsWhenAllStepsPass)
{
    auto mockImpl = std::make_unique<MockGLFWWindowImpl>();
    auto *mock = mockImpl.get();
    auto *fakeWindow = reinterpret_cast<GLFWwindow *>(0xBEEF);

    EXPECT_CALL(*mock, init()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mock, createWindow(1024, 768, ::testing::_)).WillOnce(::testing::Return(fakeWindow));
    EXPECT_CALL(*mock, makeContextCurrent(fakeWindow));
    EXPECT_CALL(*mock, loadGlad()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mock, terminate());

    Engine::Window window(std::move(mockImpl));

    EXPECT_TRUE(window.is_open());
    EXPECT_EQ(window.get(), fakeWindow);
    EXPECT_TRUE(window.isInitialized());
}

TEST(WindowTest, CanClose)
{
    auto mockImpl = std::make_unique<MockGLFWWindowImpl>();
    auto *mock = mockImpl.get();
    auto *fakeWindow = reinterpret_cast<GLFWwindow *>(0xBEEF);

    EXPECT_CALL(*mock, init()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mock, createWindow(1024, 768, ::testing::_)).WillOnce(::testing::Return(fakeWindow));
    EXPECT_CALL(*mock, makeContextCurrent(fakeWindow));
    EXPECT_CALL(*mock, loadGlad()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mock, terminate());

    Engine::Window window(std::move(mockImpl));
    window.close();

    EXPECT_EQ(window.get(), fakeWindow);
    EXPECT_TRUE(window.isInitialized());
}