// AssemblyScript - compile to spinner.wasm

// Import the host log(ptr, len). 
@external("env", "log")
declare function hostLog(ptr: i32, len: i32): void;

function log(msg: string): void {
    const buf = String.UTF8.encode(msg);
    hostLog(changetype<i32>(buf), buf.byteLength);
}

let angle: f32 = 0;

export function onStart(): void {
    angle = 0;
    log("Spinner started");
}

export function onUpdate(deltaTime: f32): void {
    angle += 90.0 * deltaTime; // Rotate 90 degrees per second
}