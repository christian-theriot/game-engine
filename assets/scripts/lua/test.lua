local Component = {}

function Component.onStart(self)
    log("Component started")
end

function Component.onUpdate(self, deltaTime)
    log("Component updated with deltaTime: " .. deltaTime)
end

return Component