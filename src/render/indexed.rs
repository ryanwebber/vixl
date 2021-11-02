use super::{buffer::{AsBuffer, VertextBuffer}, shader::{AsFragmentState, AsVertexState}};

pub struct IndexedRenderer<'a, Contents> {
    contents: Contents,
    material: super::Material<'a>,
}

impl<'a, Contents: VertextBuffer> super::Bindable for IndexedRenderer<'a, Contents> {
    fn descriptor(&self, context: &super::Context) -> wgpu::RenderPipelineDescriptor {
        let vertex_layout = Contents::layout();
        let render_pipeline_layout =
            context.device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("Render Pipeline Layout"),
                bind_group_layouts: &[],
                push_constant_ranges: &[],
            });

        wgpu::RenderPipelineDescriptor {
            label: Some("Render Pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: self.material.vertex_shader.to_vertex_state(&context.device, &[vertex_layout]),
            fragment: Some(self.material.fragment_shader.to_fragment_state(&context.device, context.swap_descriptor.format)),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList,
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw,
                cull_mode: wgpu::CullMode::Back,
                // Setting this to anything other than Fill requires Features::NON_FILL_POLYGON_MODE
                polygon_mode: wgpu::PolygonMode::Fill,
            },
            depth_stencil: None,
            multisample: wgpu::MultisampleState {
                count: 1,
                mask: !0,
                alpha_to_coverage_enabled: false,
            },
        }
    }
}

impl<'a, Contents: AsBuffer> super::Renderable for IndexedRenderer<'a,Contents> {
    fn render(&self, render_pass: &wgpu::RenderPass) {
        todo!();
    }
}
