pub mod buffer;
pub mod context;
pub mod indexed;
pub mod material;
pub mod shader;

pub use buffer::VertextBuffer;
pub use context::Context;
pub use indexed::IndexedRenderer;
pub use material::Material;
pub use shader::Shader;

trait Bindable {
    fn descriptor(&self, context: &context::Context) -> wgpu::RenderPipelineDescriptor;
}

trait Renderable {
    fn render(&self, render_pass: &wgpu::RenderPass);
}
