#ifndef __EDITOR_SCENE_HPP
#define __EDITOR_SCENE_HPP

#include <editor/mesh.hpp>
#include <editor/transform.hpp>
#include <editor/camera.hpp>
#include <memory>
#include <vector>

namespace Editor
{
    /**
     * TODO:
     * 1. Extract Transform class to be standalone outside of Scene
     */
    class SceneNode
    {
        std::vector<std::unique_ptr<SceneNode>> children;
        SceneNode *parent;
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<Transform> transform;

    public:
        SceneNode(std::unique_ptr<Mesh> mesh = nullptr);
        ~SceneNode() = default;

        void addChild(std::unique_ptr<SceneNode> child);
        void removeChild(SceneNode *child);
        SceneNode *getChild(size_t index) const;
        size_t getChildCount() const;
        SceneNode *getParent() const;

        Transform *getTransform();
        const Transform *getTransform() const;

        Mesh *getMesh() const;
        void setMesh(std::unique_ptr<Mesh> mesh);

        void update(float deltaTime, const glm::mat4 &parentMatrix);
        void render(const glm::mat4 &viewProjection, const Camera &camera);
    };

    class Scene
    {
        std::unique_ptr<SceneNode> root;
        std::string name;
        bool isActive;

    public:
        Scene(const std::string &name = "Default Scene");
        ~Scene() = default;

        std::string getName() const;
        void setActive(bool active);
        bool getActive() const;

        SceneNode *getRoot() const;
        void addNode(std::unique_ptr<SceneNode> node);

        void update(float deltaTime);
        void render(const glm::mat4 &viewProjection, const Camera &camera);
        void clear();
    };
}

#endif