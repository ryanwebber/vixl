#include <memory>

#include <App/Workspace.h>

static WorkspaceIdentifier g_WorkspaceIdentifier = 0;

std::shared_ptr<Workspace> Workspace::Create(SizeInt size) {
    auto workspace = new Workspace(++g_WorkspaceIdentifier, size);
    return std::shared_ptr<Workspace>(workspace);
}
