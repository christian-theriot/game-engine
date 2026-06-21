#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <engine/entity.hpp>

class MockComponent : public Engine::Component
{
public:
    MOCK_METHOD(void, onAttach, (), (override));
    MOCK_METHOD(void, onDetach, (), (override));
    MOCK_METHOD(void, update, (float deltaTime), (override));
};

class SecondMockComponent : public Engine::Component
{
public:
    MOCK_METHOD(void, onAttach, (), (override));
    MOCK_METHOD(void, onDetach, (), (override));
    MOCK_METHOD(void, update, (float deltaTime), (override));
};

TEST(EntityTest, CanAddAndGetComponent)
{
    Engine::Entity entity;
    MockComponent *component = entity.addComponent<MockComponent>();

    EXPECT_EQ(entity.getId(), 1);
    EXPECT_TRUE(entity.hasComponent<MockComponent>());
    EXPECT_EQ(component, entity.getComponent<MockComponent>());
}

TEST(EntityTest, CanRemoveComponent)
{
    Engine::Entity entity;
    MockComponent *component = entity.addComponent<MockComponent>();

    EXPECT_TRUE(entity.hasComponent<MockComponent>());

    entity.removeComponent<MockComponent>();

    EXPECT_FALSE(entity.hasComponent<MockComponent>());
}

TEST(EntityTest, CanGetComponents)
{
    Engine::Entity entity;
    MockComponent *component1 = entity.addComponent<MockComponent>();
    SecondMockComponent *component2 = entity.addComponent<SecondMockComponent>();

    const auto &components = entity.getComponents();

    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(components.at(typeid(MockComponent).hash_code()).get(), component1);
    EXPECT_EQ(components.at(typeid(SecondMockComponent).hash_code()).get(), component2);
}