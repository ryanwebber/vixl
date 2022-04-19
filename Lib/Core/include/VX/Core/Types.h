#pragma once

#include <VX/Core/Handle.h>

namespace VX::Core {

    struct HandleStub {
        static size_t s_index;

        size_t idx;

        HandleStub()
            : idx(++s_index)
        {
        }

        inline void destroy()
        {
        }
    };

    typedef Handle<HandleStub> ShaderHandle;
    typedef Handle<HandleStub> ProgramHandle;
    typedef Handle<HandleStub> TextureHandle;
    typedef Handle<HandleStub> VertexBufferHandle;
    typedef Handle<HandleStub> IndexBufferHandle;
    typedef Handle<HandleStub> UniformHandle;
}
