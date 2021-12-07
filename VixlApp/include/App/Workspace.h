#pragma once

typedef int WorkspaceIdentifier;

class Workspace {
private:
    WorkspaceIdentifier m_Identifier;

    explicit Workspace(WorkspaceIdentifier identifier)
        : m_Identifier(identifier)
        { }

public:

    ~Workspace() = default;
    [[nodiscard]] WorkspaceIdentifier GetIdentifier() const { return m_Identifier; }

    static std::shared_ptr<Workspace> Create();
};
