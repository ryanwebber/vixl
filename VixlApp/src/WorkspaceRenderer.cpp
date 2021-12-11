#include <Glad/gl.h>

#include <glm/gtx/string_cast.hpp>

#include <App/WorkspaceRenderer.h>
#include <App/Logger.h>
#include <App/Painter.h>
#include <App/Camera.h>

void RenderScene(Workspace&, glm::mat4x4);

GLuint vertex_shader;
GLuint fragment_shader;
GLuint shader_program;
GLuint vertex_buffer;
GLuint vertex_array;

char infoLog[512];
float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
};

const char *vertex_shader_source = R"glsl(
#version 330 core

layout (location = 0) in vec3 aPos;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)glsl";

const char *fragment_shader_source = R"glsl(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)glsl";

void WorkspaceRenderer::Initialize() {
    auto[width,height] = m_Workspace->GetViewport().GetSize().cast<int>().dimensions;
//
    // Initialize  frame buffer
    glGenFramebuffers(1, &m_GLFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_GLFrameBuffer);

    // Initialize a texture buffer
    glGenTextures(1, &m_GLTextureBuffer);
    glBindTexture(GL_TEXTURE_2D, m_GLTextureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind the texture buffer to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GLTextureBuffer, 0);

    // Initialize a render buffer
    glGenRenderbuffers(1, &m_GLRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_GLRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_GLRenderBuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::Core->debug("Unable to build framebuffer: status={}", glCheckFramebufferStatus(GL_FRAMEBUFFER));

    // Unbind the framebuffer so we don't do further drawing to it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Logger::Core->debug("Finished initializing graphics buffers for workspace {}", m_Workspace->GetIdentifier());
    m_Workspace->GetViewport().SetTexture(reinterpret_cast<void*>(m_GLTextureBuffer));


    // --------- Test drawing --------------

    int success;

    // Vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
        Logger::Core->critical("Unable to compile shader: {}", infoLog);
        return;
    }

    // Fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    // check for shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
        Logger::Core->critical("Unable to compile shader: {}", infoLog);
        return;
    }

    // link shaders
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
        Logger::Core->debug("Error linking shaders: {}", infoLog);
    }

    // Cleanup shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Generate buffers
    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void WorkspaceRenderer::Render() const {
    const auto[r,g,b,a] = Colors::workspace_background.rgba;

    // First pass: draw scene to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_GLFrameBuffer);
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);

    RenderScene(*m_Workspace, glm::mat4x4());

    // Unbind the framebuffer to avoid further drawing to it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WorkspaceRenderer::Destroy() {
    Logger::Core->debug("Deallocating rendering resources for workspace {}", m_Workspace->GetIdentifier());
    // TODO: destroy GL memory
}

void RenderScene(Workspace& workspace, glm::mat4x4 _) {
    glUseProgram(shader_program);

    auto transform = Camera().GetProjection(workspace.GetViewport().GetSize());
//    auto transform = glm::mat4x4(1.0f);
    GLint uniform_location = glGetUniformLocation(shader_program, "transform");
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &transform[0][0]);

    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    Logger::Core->debug("Got mvp matrix:{}", glm::to_string(transform));
}
