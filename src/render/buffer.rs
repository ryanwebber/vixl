pub trait AsBuffer {
    fn contents<'a>(&self) -> &'a [u8];
}

pub trait VertextBuffer {
    fn layout<'a>() -> wgpu::VertexBufferLayout<'a>;
}
