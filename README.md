# Game Engine - Name TBD
## Technologies
- C++
- Jolt Physics
- Wasm
- Lua

## Features
- Entity Component System (ECS) comprising:
  - TransformComponent, TransformSystem
  - RigidbodyComponent, PhysicsSystem
  - LuaScriptComponent, LuaScriptingSystem
  - WasmScriptComponent, WasmScriptingSystem
  - MeshComponent, RenderSystem
- Audio
- Input Management
- Perspective Camera
- Clock with high resolution deltaTime, and variable timescale (including negative for rewind effects)
- Image loading (BMP, PNG, and other formats supported by stbi_load)
- Material loading with custom shaders, and a single texture
- Primitive Meshes
  - Cube
  - Sphere
  - Plane
  - Line
- Basic versioning using `major` . `minor` . `patch`

## Development
1. `make setup` to configure the code using CMake
   - If you want to enable test coverage, run `make setup-tests`
   - After step 2, run `make coverage` to generate a test coverage report
   - Then, you can run `make show-coverage` to open the report in the default browser
2. `make` to build all dependencies
3. `make run` to run the game engine
4. You can optionally debug the application
   - `make debug`
   - `run`
   - This gives you an lldb execution of the application, so you can run stack traces, set breakpoints, etc.