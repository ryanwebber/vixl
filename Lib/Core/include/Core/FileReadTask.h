#pragma once

#include <filesystem>
#include <memory>
#include <span>
#include <string>

#include <uvw/fs.h>

#include <Common/Error.h>

#include <Core/TaskFuture.h>

namespace Core {
    typedef Common::Expected<std::span<const char>> FileReadResult;

    class FileReadTask final : public TaskFuture<FileReadResult> {
        VX_DEFAULT_MOVABLE(FileReadTask);
    private:
        std::shared_ptr<uvw::FileReq> m_Handle;

        std::filesystem::path m_FilePath;
        size_t m_Offset;
        size_t m_Length;
    protected:
        std::unique_ptr<Closable> Dispatch() override;
        std::unique_ptr<Closable> AttachListener(std::function<void(FileReadResult)> listener) override;

    public:
        FileReadTask(std::shared_ptr<EventLoop>, std::filesystem::path filepath, size_t offset, size_t length);
        ~FileReadTask() = default;
    };
}
