#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <engine/bindings.hpp>
#include <engine/world.hpp>
#include <engine/entity.hpp>
#include <engine/components/transform.hpp>

// Lua Bindings Test Fixture

class LuaBindingsTest : public ::testing::Test
{
protected:
    sol::state lua;
    Engine::World world;

    void SetUp() override
    {
        lua.open_libraries(sol::lib::base);
        Engine::LuaBindings().bindAll(lua, &world);
    }

    Engine::Entity *createEntityWithTransform()
    {
        Engine::Entity *entity = world.createEntity();
        entity->addComponent<Engine::Components::TransformComponent>();
        return entity;
    }
};

TEST_F(LuaBindingsTest, LogFunctionExists)
{
    auto result = lua.safe_script(R"(log("Hello from Lua"))");
    EXPECT_TRUE(result.valid());
}

TEST_F(LuaBindingsTest, Vec3TypeExists)
{
    auto result = lua.safe_script(R"(local v = vec3.new(1, 2, 3); return v.x, v.y, v.z)");
    EXPECT_TRUE(result.valid());
    auto tuple = result.get<std::tuple<float, float, float>>();
    EXPECT_EQ(std::get<0>(tuple), 1.0f);
    EXPECT_EQ(std::get<1>(tuple), 2.0f);
    EXPECT_EQ(std::get<2>(tuple), 3.0f);
}

TEST_F(LuaBindingsTest, EntityFindByIdResturnsNilForMissingEntity)
{
    auto result = lua.safe_script(R"(return Entity.findById(999))");
    ASSERT_TRUE(result.valid());
    EXPECT_TRUE(result.get<Engine::Entity *>() == nullptr);
}

TEST_F(LuaBindingsTest, EntityFindByIdReturnsEntity)
{
    Engine::Entity *entity = createEntityWithTransform();
    auto id = entity->getId();

    lua["targetId"] = id;
    auto result = lua.safe_script(R"(local e = Entity.findById(targetId); return e ~= nil)");

    ASSERT_TRUE(result.valid());
    EXPECT_TRUE(result.get<bool>());
}

TEST_F(LuaBindingsTest, EntitySetPositionUpdatesTransform)
{
    Engine::Entity *entity = createEntityWithTransform();
    auto id = entity->getId();

    lua["targetId"] = id;
    lua["newPos"] = glm::vec3(5.0f, 0.0f, 0.0f);
    auto result = lua.safe_script(R"(local e = Entity.findById(targetId); e:setPosition(newPos))");

    ASSERT_TRUE(result.valid());
    auto transform = entity->getComponent<Engine::Components::TransformComponent>();
    EXPECT_EQ(transform->getTransform()->getPosition(), glm::vec3(5.0f, 0.0f, 0.0f));
}

// Wasm Bindings Test Fixture

// These host functions are declared in bindings.cpp.
// Forward declarations so tests can call them directly.
wasm_trap_t *getEntityById(void *env, wasmtime_caller_t *caller,
                           const wasmtime_val_t *args, size_t nargs,
                           wasmtime_val_t *results, size_t nresults);
wasm_trap_t *setEntityPosition(void *env, wasmtime_caller_t *caller,
                               const wasmtime_val_t *args, size_t nargs,
                               wasmtime_val_t *results, size_t nresults);
wasm_trap_t *hostAbort(void *env, wasmtime_caller_t *caller,
                       const wasmtime_val_t *args, size_t nargs,
                       wasmtime_val_t *results, size_t nresults);

class WasmBindingsTest : public ::testing::Test
{
protected:
    Engine::World world;

    Engine::Entity *createEntityWithTransform()
    {
        Engine::Entity *entity = world.createEntity();
        entity->addComponent<Engine::Components::TransformComponent>();
        return entity;
    }

    static wasmtime_val_t i32(int32_t value)
    {
        wasmtime_val_t val;
        val.kind = WASMTIME_I32;
        val.of.i32 = value;
        return val;
    }

    static wasmtime_val_t f32(float value)
    {
        wasmtime_val_t val;
        val.kind = WASMTIME_F32;
        val.of.f32 = value;
        return val;
    }
};

TEST_F(WasmBindingsTest, GetEntityByIdReturnsIdWhenExists)
{
    Engine::Entity *entity = createEntityWithTransform();
    auto id = entity->getId();

    wasmtime_val_t args[1] = {i32(id)};
    wasmtime_val_t results[1];

    auto *trap = getEntityById(&world, nullptr, args, 1, results, 1);

    EXPECT_EQ(trap, nullptr);
    EXPECT_EQ(results[0].kind, WASMTIME_I32);
    EXPECT_EQ(results[0].of.i32, id);
}

TEST_F(WasmBindingsTest, GetEntityByIdReturnsZeroForMissingEntity)
{
    wasmtime_val_t args[1] = {i32(99999)};
    wasmtime_val_t results[1];

    wasm_trap_t *trap = getEntityById(&world, nullptr, args, 1, results, 1);

    EXPECT_EQ(trap, nullptr);
    EXPECT_EQ(results[0].kind, WASMTIME_I32);
    EXPECT_EQ(results[0].of.i32, 0); // 0 = not found
}

TEST_F(WasmBindingsTest, GetEntityByIdReturnsNullTrapOnWrongArgCount)
{
    wasmtime_val_t results[1];
    // nargs = 0 (wrong) — function returns early, results untouched
    wasm_trap_t *trap = getEntityById(&world, nullptr, nullptr, 0, results, 1);
    EXPECT_EQ(trap, nullptr); // no trap, just early return
}

TEST_F(WasmBindingsTest, SetEntityPositionUpdatesTransform)
{
    Engine::Entity *entity = createEntityWithTransform();
    int32_t id = static_cast<int32_t>(entity->getId());

    wasmtime_val_t args[4] = {
        i32(id), f32(1.0f), f32(2.0f), f32(3.0f)};

    wasm_trap_t *trap = setEntityPosition(&world, nullptr, args, 4, nullptr, 0);

    EXPECT_EQ(trap, nullptr);

    auto *transform = entity->getComponent<Engine::Components::TransformComponent>();
    glm::vec3 pos = transform->getTransform()->getPosition();
    EXPECT_FLOAT_EQ(pos.x, 1.0f);
    EXPECT_FLOAT_EQ(pos.y, 2.0f);
    EXPECT_FLOAT_EQ(pos.z, 3.0f);
}

TEST_F(WasmBindingsTest, SetEntityPositionOnMissingEntityDoesNotCrash)
{
    wasmtime_val_t args[4] = {
        i32(99999), f32(1.0f), f32(2.0f), f32(3.0f)};

    wasm_trap_t *trap = setEntityPosition(&world, nullptr, args, 4, nullptr, 0);

    EXPECT_EQ(trap, nullptr); // graceful, no crash
}

TEST_F(WasmBindingsTest, SetEntityPositionWrongArgCountReturnsEarly)
{
    wasmtime_val_t args[2] = {i32(1), f32(1.0f)};
    wasm_trap_t *trap = setEntityPosition(&world, nullptr, args, 2, nullptr, 0);
    EXPECT_EQ(trap, nullptr); // early return, no crash
}

TEST_F(WasmBindingsTest, HostAbortReturnsNullTrap)
{
    wasm_trap_t *trap = hostAbort(nullptr, nullptr, nullptr, 0, nullptr, 0);
    EXPECT_EQ(trap, nullptr);
}