## 1. Audio System - √ Complete
Add sound effects/music playback

## 2. Input System - √ Complete
Keyboard/mouse controls for entities

## 3. Scripting System
C++/Julia scripting for gameplay

## 4. Optimization
Spatial partinioning for 1000+ physics objects

## 5. Advanced Graphics
Lighting, shadows, post-processing

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
- Scripting System
- Optimization
- Advanced Graphics
  - Specular / Diffuse / Ambient
  - Normal Maps
  - MipMaps