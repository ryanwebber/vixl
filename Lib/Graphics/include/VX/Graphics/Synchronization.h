#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>
#include <VX/Graphics/APIObject.h>

namespace VX::Graphics {
    namespace Private {
        class SemaphoreImpl;
        class FenceImpl;
    }

    class Semaphore final : public APIObject<Private::SemaphoreImpl> {
    public:
        using APIObject<Private::SemaphoreImpl>::APIObject;
    };

    class Fence final : public APIObject<Private::FenceImpl> {
    public:
        using APIObject<Private::FenceImpl>::APIObject;
    };
}
