#include <editor/scene.hpp>
#include <algorithm>

Editor::SceneNode::SceneNode(std::unique_ptr<Mesh> mesh)
    : parent(nullptr),
      mesh(std::move(mesh)),
      transform(std::make_unique<Transform>())
{
}
void Editor::SceneNode::addChild(std::unique_ptr<SceneNode> child)
{
    if (child)
    {
        child->parent = this;
        children.push_back(std::move(child));
    }
}
void Editor::SceneNode::removeChild(SceneNode *child)
{
    auto item = std::find_if(children.begin(), children.end(), [child](const std::unique_ptr<SceneNode> &node)
                             { return node.get() == child; });

    if (item != children.end())
    {
        children.erase(item);
    }
}
Editor::SceneNode *Editor::SceneNode::getChild(size_t index) const
{
    if (index < children.size())
    {
        return children[index].get();
    }

    return nullptr;
}
size_t Editor::SceneNode::getChildCount() const
{
    return children.size();
}
Editor::SceneNode *Editor::SceneNode::getParent() const
{
    return parent;
}
Editor::Transform *Editor::SceneNode::getTransform()
{
    return transform.get();
}
const Editor::Transform *Editor::SceneNode::getTransform() const
{
    return transform.get();
}
Editor::Mesh *Editor::SceneNode::getMesh() const
{
    return mesh.get();
}
void Editor::SceneNode::setMesh(std::unique_ptr<Mesh> newMesh)
{
    mesh = std::move(newMesh);
}
void Editor::SceneNode::update(float deltaTime, const glm::mat4 &parentMatrix)
{
    transform->update(parentMatrix);

    for (auto &child : children)
    {
        child->update(deltaTime, transform->getWorldMatrix());
    }
}
void Editor::SceneNode::render(const glm::mat4 &viewProjection, const Camera &camera)
{
    if (mesh)
    {
        camera.render(*mesh.get(), *transform.get());
    }

    for (auto &child : children)
    {
        child->render(viewProjection, camera);
    }
}

Editor::Scene::Scene(const std::string &name)
    : name(name), isActive(true)
{
    root = std::make_unique<SceneNode>();
}
std::string Editor::Scene::getName() const
{
    return name;
}
void Editor::Scene::setActive(bool active)
{
    isActive = active;
}
bool Editor::Scene::getActive() const
{
    return isActive;
}
Editor::SceneNode *Editor::Scene::getRoot() const
{
    return root.get();
}
void Editor::Scene::addNode(std::unique_ptr<SceneNode> node)
{
    if (root && node)
    {
        root->addChild(std::move(node));
    }
}
void Editor::Scene::update(float deltaTime)
{
    if (!isActive || !root)
    {
        return;
    }

    root->update(deltaTime, glm::mat4(1.f));
}
void Editor::Scene::render(const glm::mat4 &viewProjection, const Camera &camera)
{
    if (!isActive || !root)
    {
        return;
    }

    root->render(viewProjection, camera);
}
void Editor::Scene::clear()
{
    root = std::make_unique<SceneNode>();
}