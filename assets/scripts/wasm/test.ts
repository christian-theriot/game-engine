@external("env", "abort")
declare function abort(msg: string | null, fileName: string | null, lineNumber: u32, columnNumber: u32): void;

@external("env", "log")
declare function log(ptr: i32, len: i32): void;

function print(msg: string): void {
    const buf = String.UTF8.encode(msg);
    log(changetype<i32>(buf), buf.byteLength);
    abort("abort", "test.ts", 10, 30);
}

export function onStart(): void {
    print("Test started");
}

export function onUpdate(deltaTime: f32): void {
    print("Test update: " + deltaTime.toString());
}