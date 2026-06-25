local Spinner = {entity = 0}

function Spinner.onStart(self)
    self.angle = 0
    self.entity = Entity.findById(1)
    log("Spinner started")
end

local theta = 0.0
local phi = 0.0
local scaleX = 1.0
function Spinner.onUpdate(self, deltaTime)
    theta = theta + (math.pi / 4) *deltaTime
    phi = phi + (math.pi / 4) * deltaTime
    
    if scaleX > 1.5 then
        scaleX = scaleX - deltaTime * 0.5
    else
        scaleX = scaleX + deltaTime * 0.5
    end
    -- self.entity:setScale(vec3.new(scaleX, 1.0, 1.0))

    local pos = vec3.new(math.cos(theta) * 2, 0, math.sin(theta) * 2)
    local rot = vec3.new(0, phi, phi * phi)
    -- self.entity:setPosition(pos)
    -- self.entity:setRotation(rot)
end

return Spinner