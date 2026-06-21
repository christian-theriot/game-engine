// AssemblyScript - compile to spinner.wasm

// Import the host log(ptr, len). 
@external("env", "log")
declare function hostLog(ptr: i32, len: i32): void;

@external("env", "getEntityById")
declare function getEntityById(id: i32): i32;

@external("env", "setEntityPosition")
declare function setEntityPosition(entityPtr: i32, x: f32, y: f32, z: f32): void;

@external("env", "setEntityRotation")
declare function setEntityRotation(entityPtr: i32, x: f32, y: f32, z: f32): void;

@external("env", "setEntityScale")
declare function setEntityScale(entityPtr: i32, x: f32, y: f32, z: f32): void;

function log(msg: string): void {
    const buf = String.UTF8.encode(msg);
    hostLog(changetype<i32>(buf), buf.byteLength);
}

let angle: f32 = 0;
let scaleX: f32 = 0.01;

export function onStart(): void {
    angle = 0;
    log("Spinner started");
}

let increment = 1;
export function onUpdate(deltaTime: f32): void {
    angle += 90.0 * deltaTime; // Rotate 90 degrees per second
    scaleX += deltaTime * 0.5 * f32(increment);
    if (scaleX > 1.5) {
        increment = -1;
    } else if (scaleX < 0.5) {
        increment = 1;
    }
    const entity = getEntityById(1);

    if (entity == 1) {
        setEntityPosition(entity, f32(Math.cos(angle * (Math.PI / 180))), 0, f32(Math.sin(angle * (Math.PI / 180))));
        setEntityRotation(entity, 0, f32(angle * (Math.PI / 180)), 0);
        setEntityScale(entity, scaleX, 1.0, 1.0);
    }
}