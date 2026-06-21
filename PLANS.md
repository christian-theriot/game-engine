## 1. Audio System - √ Complete
Add sound effects/music playback

## 2. Input System - √ Complete
Keyboard/mouse controls for entities

## 3. Scripting System - √ Complete
Lua (sol2) scripting for gameplay - WebAssembly backend planned next

## 4. Advanced Graphics
Lighting, shadows, post-processing

## 5. Data Serialization - 🚧 In Progress
- Serialize components
- Serialize entities
- Unserialize entities

## 6. Immediate Mode Editor
- Panels for creating, editing, saving scenes
- Creating & moving objects, setting properties on their components
- docking of panels, with an OpenGL context for the editor/game preview
- text editing
- export scene to file
- import scene from file

## Code Quality Improvements
- Remove magic numbers → Create constants for camera bounds, speeds
- Add error handling → Check file loading failures, GL errors
- Separate concerns → Split render() into handleInput(), update(), draw()
- Use smart pointers → std::unique_ptr<Entity> instead of raw pointers
- Configuration file → YAML/JSON for settings instead of hardcoded values
## Priority Order
- <s>Delta time (immediate)</s> - ✓ Complete
- <s>Resource manager (quick win)</s> - ✓ Complete
- <s>Scene system (enables growth)</s> - ✓ Complete
- <s>Physics/Audio (feature expansion)</s> - √ Complete
- <s>ECS (foundational refactor)</s> - √ Complete
- <s>Event System</s> - √ Complete
- <s>Input System</s> - √ Complete
- <s>Audio System</s> - √ Complete
- Scripting System - 🚧 In Progress
  - <s>Lua (sol2)</s> - √ Complete
  - <s>WebAssembly (Wasmtime)</s> - √ Complete
  - <s>Runtime + loading + instantiation</s> - √ Complete
  - Entity manipulation
    - setPosition
    - setRotation
    - setScale
- Data serialization - 🚧 In Progress
- Immediate mode Editor
- Advanced Graphics
  - Specular / Diffuse / Ambient
  - Normal Maps
  - MipMaps