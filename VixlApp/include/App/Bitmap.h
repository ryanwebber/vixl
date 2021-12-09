#pragma once

#include <App/Size.h>
#include <cinttypes>

template<typename T>
class BitmapBuffer final {
private:
    Size m_Size;
    T* m_Data;

public:

    explicit BitmapBuffer(Size size)
        : m_Size(size)
        , m_Data(nullptr)
    {};

    ~BitmapBuffer() = default;

    const T* GetData() const { return m_Data; }
    T* GetData() { return m_Data; }
};

typedef BitmapBuffer<uint32_t> Bitmap;
