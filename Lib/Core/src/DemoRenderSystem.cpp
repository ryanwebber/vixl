#include <bgfx/bgfx.h>

#include <Core/DemoRenderSystem.h>
#include <Core/Logger.h>
#include <Core/Shader.h>

#include <Core/Generated/Shader/uvtex/fragment.h>
#include <Core/Generated/Shader/uvtex/vertex.h>

static float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
};

static uint16_t indexes[] = {
        0, 1, 2,
};

// Deleteme

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
        {
                {-1.0f,  1.0f,  1.0f, 0xff000000 },
                { 1.0f,  1.0f,  1.0f, 0xff0000ff },
                {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
                { 1.0f, -1.0f,  1.0f, 0xff00ffff },
                {-1.0f,  1.0f, -1.0f, 0xffff0000 },
                { 1.0f,  1.0f, -1.0f, 0xffff00ff },
                {-1.0f, -1.0f, -1.0f, 0xffffff00 },
                { 1.0f, -1.0f, -1.0f, 0xffffffff },
        };

static const uint16_t cubeTriList[] =
{
        0, 1, 2,
        1, 3, 2,
        4, 6, 5,
        5, 6, 7,
        0, 2, 4,
        4, 2, 6,
        1, 5, 3,
        5, 7, 3,
        0, 4, 1,
        4, 5, 1,
        2, 3, 6,
        6, 3, 7,
};

bgfx::ShaderHandle loadShader(const char *shaderPath)
{
//
//    switch(bgfx::getRendererType()) {
//        case bgfx::RendererType::Noop:
//        case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
//        case bgfx::RendererType::Direct3D11:
//        case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
//        case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
//        case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
//        case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
//        case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
//        case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
//    }
//
//    size_t shaderLen = strlen(shaderPath);
//    size_t fileLen = strlen(FILENAME);
//    char *filePath = (char *)malloc(shaderLen + fileLen);
//    memcpy(filePath, shaderPath, shaderLen);
//    memcpy(&filePath[shaderLen], FILENAME, fileLen);

    FILE *file = fopen(shaderPath, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

namespace Core {
    DemoRenderSystem::DemoRenderSystem() {
        bgfx::VertexLayout vertex_layout;
        vertex_layout
            .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        auto vb = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), vertex_layout);
        auto ib =  bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

        m_VertexBuffer = std::make_shared<VertexBuffer>(vb);
        m_IndexBuffer = std::make_shared<IndexBuffer>(ib);
        m_Material = VX_CREATE_MATERIAL("UVTexture", uvtex);

        auto vsh = std::make_shared<Shader>(loadShader("/Users/rwebber/dev/hobby/vixl-cpp/build/shaders/metal/vs_cubes.bin"));
        auto fsh = std::make_shared<Shader>(loadShader("/Users/rwebber/dev/hobby/vixl-cpp/build/shaders/metal/fs_cubes.bin"));
        m_Material = std::make_shared<Material>("MyMat", std::move(vsh), std::move(fsh));
    }

    void DemoRenderSystem::Render(RenderTarget &target, const SceneCamera &camera) {
        RenderCommand command(m_VertexBuffer, m_IndexBuffer, m_Material);
        target.Commands().push_back(command);
    }
}
