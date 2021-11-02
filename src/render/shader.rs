
pub trait AsVertexState {
    fn to_vertex_state(&self, device: &wgpu::Device, buffers: &[wgpu::VertexBufferLayout]) -> wgpu::VertexState;
}

pub trait AsFragmentState {
    fn to_fragment_state(&self, device: &wgpu::Device, format: wgpu::TextureFormat) -> wgpu::FragmentState;
}

pub struct Shader<'a> {
    descriptor: wgpu::ShaderModuleDescriptor<'a>,
    entry: &'static str,
}

impl<'a> AsVertexState for Shader<'a> {
    fn to_vertex_state(&self, device: &wgpu::Device, buffers: &[wgpu::VertexBufferLayout]) -> wgpu::VertexState {
        let module = device.create_shader_module(&self.descriptor);
        wgpu::VertexState {
            module: &module,
            entry_point: "main",
            buffers: buffers,
        }
    }
}

impl<'a> AsFragmentState for Shader<'a> {
    fn to_fragment_state(&self, device: &wgpu::Device, format: wgpu::TextureFormat) -> wgpu::FragmentState {
        let module  = device.create_shader_module(&self.descriptor);
        wgpu::FragmentState {
            module: &module,
            entry_point: "main",
            targets: &[wgpu::ColorTargetState {
                format,
                alpha_blend: wgpu::BlendState::REPLACE,
                color_blend: wgpu::BlendState::REPLACE,
                write_mask: wgpu::ColorWrite::ALL,
            }],
        }
    }
}
