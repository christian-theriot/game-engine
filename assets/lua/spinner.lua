local Spinner = {}

function Spinner.onStart(self)
    self.angle = 0
    log("Spinner started")
end

function Spinner.onUpdate(self, deltaTime)
    self.angle = self.angle + (90 * deltaTime)
end

return Spinner