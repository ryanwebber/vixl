#include <Glad/gl.h>

#include <App/WorkspaceRenderer.h>
#include <App/Logger.h>

void WorkspaceRenderer::Initialize() {
    const auto width = m_Workspace->GetViewport().GetSize().x;
    const auto height = m_Workspace->GetViewport().GetSize().y;

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
}

void WorkspaceRenderer::Render() const {
    // First pass: draw scene to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_GLFrameBuffer);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now

    // TODO: draw now

    // Unbind the framebuffer so we don't do further drawing to it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WorkspaceRenderer::Destroy() {
    // TODO: destroy this stuff
}
