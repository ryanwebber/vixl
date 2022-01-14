#pragma once

#include <memory>
#include <string>
#include <utility>

#include <Common/Noncopyable.h>

#include <Core/Closable.h>
#include <Core/EventLoop.h>

namespace Core {
    class Task {
    VX_MAKE_NONCOPYABLE(Task);
    VX_MAKE_NONMOVABLE(Task);

    private:
        std::string m_Name;

    protected:
        explicit Task(const std::string_view &name, const EventLoop &eventLoop)
                : m_Name(name) {}

    public:
        virtual ~Task() = default;;

        virtual void Clear() = 0;

        virtual void Close() = 0;

        [[nodiscard]] const std::string &GetName() const { return m_Name; }
    };
}