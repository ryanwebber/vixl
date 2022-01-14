#include <uvw/fs.h>

#include <Common/Error.h>
#include <Common/Expected.h>
#include <Core/FileReadTask.h>

namespace Core {

    FileReadTask::FileReadTask(std::shared_ptr<EventLoop> event_loop,
                               std::filesystem::path filepath,
                               size_t offset,
                               size_t length)
        : TaskFuture<FileReadResult>(std::move(event_loop))
        , m_FilePath(std::move(filepath))
        , m_Offset(offset)
        , m_Length(length)
    {
        m_Handle = m_EventLoop->GetInternal()->resource<uvw::FileReq>();
    }

    std::unique_ptr<Closable> FileReadTask::Dispatch() {
        m_Handle->read(m_Offset, m_Length);
        auto closable = std::make_unique<Closable>([handle = std::move(m_Handle)]() {
            handle->clear();
            handle->close();
        });

        return std::move(closable);
    }

    std::unique_ptr<Closable> FileReadTask::AttachListener(std::function<void(FileReadResult)> listener) {
        auto ok_conn = m_Handle->once<uvw::FsEvent<uvw::FileReq::Type::READ>>([&](auto &read, auto&) {
            std::span<const char> view(&read.data[0], read.size);
            listener(view);
        });

        auto err_conn = m_Handle->once<uvw::ErrorEvent>([&](auto &error, auto&) {
            listener(Common::MakeUnexpected<std::span<const char>>(error.what()));
        });

        auto closable = std::make_unique<Closable>([=]() {
            (void)ok_conn;
            (void)err_conn;
        });

        return std::move(closable);
    }
}
