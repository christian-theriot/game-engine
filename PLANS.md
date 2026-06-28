## 1. Advanced Graphics
Lighting, shadows, post-processing

## 2. Immediate Mode Editor - 🚧 In Progress
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
- <s>Physics/Audio (feature expansion)</s> - ✓ Complete
- <s>ECS (foundational refactor)</s> - ✓ Complete
- <s>Event System</s> - ✓ Complete
- <s>Input System</s> - ✓ Complete
- <s>Audio System</s> - ✓ Complete
- <s>Scripting System</s> - ✓ Complete
  - <s>Lua (sol2)</s> - ✓ Complete
  - <s>WebAssembly (Wasmtime)</s> - ✓ Complete
  - <s>Runtime + loading + instantiation</s> - ✓ Complete
  - <s>Entity manipulation</s> - ✓ Complete
    - <s>setPosition</s> - ✓ Complete
    - <s>setRotation</s> - ✓ Complete
    - <s>setScale</s> - ✓ Complete
- <s>Data serialization</s> - ✓ Complete
- Immediate mode Editor - 🚧 In Progress
- Advanced Graphics
  - Specular / Diffuse / Ambient
  - Normal Maps
  - MipMaps