local Spinner = {entity = 0}

function Spinner.onStart(self)
    self.angle = 0
    self.entity = Entity.findById(1)
    log("Spinner started")
end

local theta = 0.0
function Spinner.onUpdate(self, deltaTime)
    theta = theta + (math.pi / 4) *deltaTime

    local pos = vec3.new(math.cos(theta) * 2, 0, math.sin(theta) * 2)
    -- self.entity:setPosition(pos)
end

return Spinner