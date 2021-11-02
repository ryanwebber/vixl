#[macro_export]
macro_rules! include_compiled_shader {
    ($path:expr) => {
        {
            wgpu::include_spirv!(concat!(env!("VIXL_COMPILED_SHADER_DIR"), "/", $path, ".spv"))
        }
    };
}
