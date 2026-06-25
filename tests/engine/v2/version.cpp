#include <gtest/gtest.h>
#include <engine/v2/version.hpp>

TEST(VersionTest, GetVersion)
{
    auto version = Engine::Version(1, 2, 3);
    auto versionStr = version.get();

    ASSERT_FALSE(versionStr.empty());
    EXPECT_EQ(versionStr, "1.2.3");
}