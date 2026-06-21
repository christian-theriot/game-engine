#include <engine/version.hpp>
#include <gtest/gtest.h>

TEST(VersionTest, GetVersionString)
{
    Engine::Version version(1, 2, 3);
    EXPECT_EQ(version.get(), "1.2.3");
}