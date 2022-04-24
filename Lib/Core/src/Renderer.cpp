#include <array>
#include <span>

#include <VX/Overloaded.h>
#include <VX/Core/Assert.h>
#include <VX/Core/Renderer.h>

namespace Example {
    const std::array<unsigned char, 1504> vertex_shader_source = {
            0x03, 0x02, 0x23, 0x07,   0x00, 0x00, 0x01, 0x00,   0x0a, 0x00, 0x0d, 0x00,   0x36, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,   0x11, 0x00, 0x02, 0x00,   0x01, 0x00, 0x00, 0x00,   0x0b, 0x00, 0x06, 0x00,
            0x01, 0x00, 0x00, 0x00,   0x47, 0x4c, 0x53, 0x4c,   0x2e, 0x73, 0x74, 0x64,   0x2e, 0x34, 0x35, 0x30,
            0x00, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x03, 0x00,   0x00, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,
            0x0f, 0x00, 0x08, 0x00,   0x00, 0x00, 0x00, 0x00,   0x04, 0x00, 0x00, 0x00,   0x6d, 0x61, 0x69, 0x6e,
            0x00, 0x00, 0x00, 0x00,   0x22, 0x00, 0x00, 0x00,   0x26, 0x00, 0x00, 0x00,   0x31, 0x00, 0x00, 0x00,
            0x03, 0x00, 0x03, 0x00,   0x02, 0x00, 0x00, 0x00,   0xc2, 0x01, 0x00, 0x00,   0x04, 0x00, 0x0a, 0x00,
            0x47, 0x4c, 0x5f, 0x47,   0x4f, 0x4f, 0x47, 0x4c,   0x45, 0x5f, 0x63, 0x70,   0x70, 0x5f, 0x73, 0x74,
            0x79, 0x6c, 0x65, 0x5f,   0x6c, 0x69, 0x6e, 0x65,   0x5f, 0x64, 0x69, 0x72,   0x65, 0x63, 0x74, 0x69,
            0x76, 0x65, 0x00, 0x00,   0x04, 0x00, 0x08, 0x00,   0x47, 0x4c, 0x5f, 0x47,   0x4f, 0x4f, 0x47, 0x4c,
            0x45, 0x5f, 0x69, 0x6e,   0x63, 0x6c, 0x75, 0x64,   0x65, 0x5f, 0x64, 0x69,   0x72, 0x65, 0x63, 0x74,
            0x69, 0x76, 0x65, 0x00,   0x05, 0x00, 0x04, 0x00,   0x04, 0x00, 0x00, 0x00,   0x6d, 0x61, 0x69, 0x6e,
            0x00, 0x00, 0x00, 0x00,   0x05, 0x00, 0x05, 0x00,   0x0c, 0x00, 0x00, 0x00,   0x70, 0x6f, 0x73, 0x69,
            0x74, 0x69, 0x6f, 0x6e,   0x73, 0x00, 0x00, 0x00,   0x05, 0x00, 0x04, 0x00,   0x17, 0x00, 0x00, 0x00,
            0x63, 0x6f, 0x6c, 0x6f,   0x72, 0x73, 0x00, 0x00,   0x05, 0x00, 0x06, 0x00,   0x20, 0x00, 0x00, 0x00,
            0x67, 0x6c, 0x5f, 0x50,   0x65, 0x72, 0x56, 0x65,   0x72, 0x74, 0x65, 0x78,   0x00, 0x00, 0x00, 0x00,
            0x06, 0x00, 0x06, 0x00,   0x20, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x67, 0x6c, 0x5f, 0x50,
            0x6f, 0x73, 0x69, 0x74,   0x69, 0x6f, 0x6e, 0x00,   0x06, 0x00, 0x07, 0x00,   0x20, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00,   0x67, 0x6c, 0x5f, 0x50,   0x6f, 0x69, 0x6e, 0x74,   0x53, 0x69, 0x7a, 0x65,
            0x00, 0x00, 0x00, 0x00,   0x06, 0x00, 0x07, 0x00,   0x20, 0x00, 0x00, 0x00,   0x02, 0x00, 0x00, 0x00,
            0x67, 0x6c, 0x5f, 0x43,   0x6c, 0x69, 0x70, 0x44,   0x69, 0x73, 0x74, 0x61,   0x6e, 0x63, 0x65, 0x00,
            0x06, 0x00, 0x07, 0x00,   0x20, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0x67, 0x6c, 0x5f, 0x43,
            0x75, 0x6c, 0x6c, 0x44,   0x69, 0x73, 0x74, 0x61,   0x6e, 0x63, 0x65, 0x00,   0x05, 0x00, 0x03, 0x00,
            0x22, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x05, 0x00, 0x06, 0x00,   0x26, 0x00, 0x00, 0x00,
            0x67, 0x6c, 0x5f, 0x56,   0x65, 0x72, 0x74, 0x65,   0x78, 0x49, 0x6e, 0x64,   0x65, 0x78, 0x00, 0x00,
            0x05, 0x00, 0x05, 0x00,   0x31, 0x00, 0x00, 0x00,   0x66, 0x72, 0x61, 0x67,   0x43, 0x6f, 0x6c, 0x6f,
            0x72, 0x00, 0x00, 0x00,   0x48, 0x00, 0x05, 0x00,   0x20, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
            0x0b, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x48, 0x00, 0x05, 0x00,   0x20, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00,   0x0b, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x48, 0x00, 0x05, 0x00,
            0x20, 0x00, 0x00, 0x00,   0x02, 0x00, 0x00, 0x00,   0x0b, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,
            0x48, 0x00, 0x05, 0x00,   0x20, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0x0b, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00,   0x47, 0x00, 0x03, 0x00,   0x20, 0x00, 0x00, 0x00,   0x02, 0x00, 0x00, 0x00,
            0x47, 0x00, 0x04, 0x00,   0x26, 0x00, 0x00, 0x00,   0x0b, 0x00, 0x00, 0x00,   0x2a, 0x00, 0x00, 0x00,
            0x47, 0x00, 0x04, 0x00,   0x31, 0x00, 0x00, 0x00,   0x1e, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
            0x13, 0x00, 0x02, 0x00,   0x02, 0x00, 0x00, 0x00,   0x21, 0x00, 0x03, 0x00,   0x03, 0x00, 0x00, 0x00,
            0x02, 0x00, 0x00, 0x00,   0x16, 0x00, 0x03, 0x00,   0x06, 0x00, 0x00, 0x00,   0x20, 0x00, 0x00, 0x00,
            0x17, 0x00, 0x04, 0x00,   0x07, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,   0x02, 0x00, 0x00, 0x00,
            0x15, 0x00, 0x04, 0x00,   0x08, 0x00, 0x00, 0x00,   0x20, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
            0x2b, 0x00, 0x04, 0x00,   0x08, 0x00, 0x00, 0x00,   0x09, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,
            0x1c, 0x00, 0x04, 0x00,   0x0a, 0x00, 0x00, 0x00,   0x07, 0x00, 0x00, 0x00,   0x09, 0x00, 0x00, 0x00,
            0x20, 0x00, 0x04, 0x00,   0x0b, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,   0x0a, 0x00, 0x00, 0x00,
            0x3b, 0x00, 0x04, 0x00,   0x0b, 0x00, 0x00, 0x00,   0x0c, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x2b, 0x00, 0x04, 0x00,   0x06, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
            0x2b, 0x00, 0x04, 0x00,   0x06, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0xbf,
            0x2c, 0x00, 0x05, 0x00,   0x07, 0x00, 0x00, 0x00,   0x0f, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,
            0x0e, 0x00, 0x00, 0x00,   0x2b, 0x00, 0x04, 0x00,   0x06, 0x00, 0x00, 0x00,   0x10, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3f,   0x2c, 0x00, 0x05, 0x00,   0x07, 0x00, 0x00, 0x00,   0x11, 0x00, 0x00, 0x00,
            0x10, 0x00, 0x00, 0x00,   0x10, 0x00, 0x00, 0x00,   0x2c, 0x00, 0x05, 0x00,   0x07, 0x00, 0x00, 0x00,
            0x12, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x00, 0x00,   0x10, 0x00, 0x00, 0x00,   0x2c, 0x00, 0x06, 0x00,
            0x0a, 0x00, 0x00, 0x00,   0x13, 0x00, 0x00, 0x00,   0x0f, 0x00, 0x00, 0x00,   0x11, 0x00, 0x00, 0x00,
            0x12, 0x00, 0x00, 0x00,   0x17, 0x00, 0x04, 0x00,   0x14, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x03, 0x00, 0x00, 0x00,   0x1c, 0x00, 0x04, 0x00,   0x15, 0x00, 0x00, 0x00,   0x14, 0x00, 0x00, 0x00,
            0x09, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,   0x16, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x15, 0x00, 0x00, 0x00,   0x3b, 0x00, 0x04, 0x00,   0x16, 0x00, 0x00, 0x00,   0x17, 0x00, 0x00, 0x00,
            0x06, 0x00, 0x00, 0x00,   0x2b, 0x00, 0x04, 0x00,   0x06, 0x00, 0x00, 0x00,   0x18, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x80, 0x3f,   0x2c, 0x00, 0x06, 0x00,   0x14, 0x00, 0x00, 0x00,   0x19, 0x00, 0x00, 0x00,
            0x18, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,   0x2c, 0x00, 0x06, 0x00,
            0x14, 0x00, 0x00, 0x00,   0x1a, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,   0x18, 0x00, 0x00, 0x00,
            0x0d, 0x00, 0x00, 0x00,   0x2c, 0x00, 0x06, 0x00,   0x14, 0x00, 0x00, 0x00,   0x1b, 0x00, 0x00, 0x00,
            0x0d, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,   0x18, 0x00, 0x00, 0x00,   0x2c, 0x00, 0x06, 0x00,
            0x15, 0x00, 0x00, 0x00,   0x1c, 0x00, 0x00, 0x00,   0x19, 0x00, 0x00, 0x00,   0x1a, 0x00, 0x00, 0x00,
            0x1b, 0x00, 0x00, 0x00,   0x17, 0x00, 0x04, 0x00,   0x1d, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00,   0x2b, 0x00, 0x04, 0x00,   0x08, 0x00, 0x00, 0x00,   0x1e, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00,   0x1c, 0x00, 0x04, 0x00,   0x1f, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x1e, 0x00, 0x00, 0x00,   0x1e, 0x00, 0x06, 0x00,   0x20, 0x00, 0x00, 0x00,   0x1d, 0x00, 0x00, 0x00,
            0x06, 0x00, 0x00, 0x00,   0x1f, 0x00, 0x00, 0x00,   0x1f, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,
            0x21, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0x20, 0x00, 0x00, 0x00,   0x3b, 0x00, 0x04, 0x00,
            0x21, 0x00, 0x00, 0x00,   0x22, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0x15, 0x00, 0x04, 0x00,
            0x23, 0x00, 0x00, 0x00,   0x20, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x2b, 0x00, 0x04, 0x00,
            0x23, 0x00, 0x00, 0x00,   0x24, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,
            0x25, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x23, 0x00, 0x00, 0x00,   0x3b, 0x00, 0x04, 0x00,
            0x25, 0x00, 0x00, 0x00,   0x26, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,
            0x28, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,   0x07, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,
            0x2e, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0x1d, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,
            0x30, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0x14, 0x00, 0x00, 0x00,   0x3b, 0x00, 0x04, 0x00,
            0x30, 0x00, 0x00, 0x00,   0x31, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,
            0x33, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,   0x14, 0x00, 0x00, 0x00,   0x36, 0x00, 0x05, 0x00,
            0x02, 0x00, 0x00, 0x00,   0x04, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,
            0xf8, 0x00, 0x02, 0x00,   0x05, 0x00, 0x00, 0x00,   0x3e, 0x00, 0x03, 0x00,   0x0c, 0x00, 0x00, 0x00,
            0x13, 0x00, 0x00, 0x00,   0x3e, 0x00, 0x03, 0x00,   0x17, 0x00, 0x00, 0x00,   0x1c, 0x00, 0x00, 0x00,
            0x3d, 0x00, 0x04, 0x00,   0x23, 0x00, 0x00, 0x00,   0x27, 0x00, 0x00, 0x00,   0x26, 0x00, 0x00, 0x00,
            0x41, 0x00, 0x05, 0x00,   0x28, 0x00, 0x00, 0x00,   0x29, 0x00, 0x00, 0x00,   0x0c, 0x00, 0x00, 0x00,
            0x27, 0x00, 0x00, 0x00,   0x3d, 0x00, 0x04, 0x00,   0x07, 0x00, 0x00, 0x00,   0x2a, 0x00, 0x00, 0x00,
            0x29, 0x00, 0x00, 0x00,   0x51, 0x00, 0x05, 0x00,   0x06, 0x00, 0x00, 0x00,   0x2b, 0x00, 0x00, 0x00,
            0x2a, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x51, 0x00, 0x05, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x2c, 0x00, 0x00, 0x00,   0x2a, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x50, 0x00, 0x07, 0x00,
            0x1d, 0x00, 0x00, 0x00,   0x2d, 0x00, 0x00, 0x00,   0x2b, 0x00, 0x00, 0x00,   0x2c, 0x00, 0x00, 0x00,
            0x0d, 0x00, 0x00, 0x00,   0x18, 0x00, 0x00, 0x00,   0x41, 0x00, 0x05, 0x00,   0x2e, 0x00, 0x00, 0x00,
            0x2f, 0x00, 0x00, 0x00,   0x22, 0x00, 0x00, 0x00,   0x24, 0x00, 0x00, 0x00,   0x3e, 0x00, 0x03, 0x00,
            0x2f, 0x00, 0x00, 0x00,   0x2d, 0x00, 0x00, 0x00,   0x3d, 0x00, 0x04, 0x00,   0x23, 0x00, 0x00, 0x00,
            0x32, 0x00, 0x00, 0x00,   0x26, 0x00, 0x00, 0x00,   0x41, 0x00, 0x05, 0x00,   0x33, 0x00, 0x00, 0x00,
            0x34, 0x00, 0x00, 0x00,   0x17, 0x00, 0x00, 0x00,   0x32, 0x00, 0x00, 0x00,   0x3d, 0x00, 0x04, 0x00,
            0x14, 0x00, 0x00, 0x00,   0x35, 0x00, 0x00, 0x00,   0x34, 0x00, 0x00, 0x00,   0x3e, 0x00, 0x03, 0x00,
            0x31, 0x00, 0x00, 0x00,   0x35, 0x00, 0x00, 0x00,   0xfd, 0x00, 0x01, 0x00,   0x38, 0x00, 0x01, 0x00,
    };

    const std::array<const unsigned char, 572> fragment_shader_source = {
            0x03, 0x02, 0x23, 0x07,   0x00, 0x00, 0x01, 0x00,   0x0a, 0x00, 0x0d, 0x00,   0x13, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,   0x11, 0x00, 0x02, 0x00,   0x01, 0x00, 0x00, 0x00,   0x0b, 0x00, 0x06, 0x00,
            0x01, 0x00, 0x00, 0x00,   0x47, 0x4c, 0x53, 0x4c,   0x2e, 0x73, 0x74, 0x64,   0x2e, 0x34, 0x35, 0x30,
            0x00, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x03, 0x00,   0x00, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,
            0x0f, 0x00, 0x07, 0x00,   0x04, 0x00, 0x00, 0x00,   0x04, 0x00, 0x00, 0x00,   0x6d, 0x61, 0x69, 0x6e,
            0x00, 0x00, 0x00, 0x00,   0x09, 0x00, 0x00, 0x00,   0x0c, 0x00, 0x00, 0x00,   0x10, 0x00, 0x03, 0x00,
            0x04, 0x00, 0x00, 0x00,   0x07, 0x00, 0x00, 0x00,   0x03, 0x00, 0x03, 0x00,   0x02, 0x00, 0x00, 0x00,
            0xc2, 0x01, 0x00, 0x00,   0x04, 0x00, 0x0a, 0x00,   0x47, 0x4c, 0x5f, 0x47,   0x4f, 0x4f, 0x47, 0x4c,
            0x45, 0x5f, 0x63, 0x70,   0x70, 0x5f, 0x73, 0x74,   0x79, 0x6c, 0x65, 0x5f,   0x6c, 0x69, 0x6e, 0x65,
            0x5f, 0x64, 0x69, 0x72,   0x65, 0x63, 0x74, 0x69,   0x76, 0x65, 0x00, 0x00,   0x04, 0x00, 0x08, 0x00,
            0x47, 0x4c, 0x5f, 0x47,   0x4f, 0x4f, 0x47, 0x4c,   0x45, 0x5f, 0x69, 0x6e,   0x63, 0x6c, 0x75, 0x64,
            0x65, 0x5f, 0x64, 0x69,   0x72, 0x65, 0x63, 0x74,   0x69, 0x76, 0x65, 0x00,   0x05, 0x00, 0x04, 0x00,
            0x04, 0x00, 0x00, 0x00,   0x6d, 0x61, 0x69, 0x6e,   0x00, 0x00, 0x00, 0x00,   0x05, 0x00, 0x05, 0x00,
            0x09, 0x00, 0x00, 0x00,   0x6f, 0x75, 0x74, 0x43,   0x6f, 0x6c, 0x6f, 0x72,   0x00, 0x00, 0x00, 0x00,
            0x05, 0x00, 0x05, 0x00,   0x0c, 0x00, 0x00, 0x00,   0x66, 0x72, 0x61, 0x67,   0x43, 0x6f, 0x6c, 0x6f,
            0x72, 0x00, 0x00, 0x00,   0x47, 0x00, 0x04, 0x00,   0x09, 0x00, 0x00, 0x00,   0x1e, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,   0x47, 0x00, 0x04, 0x00,   0x0c, 0x00, 0x00, 0x00,   0x1e, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,   0x13, 0x00, 0x02, 0x00,   0x02, 0x00, 0x00, 0x00,   0x21, 0x00, 0x03, 0x00,
            0x03, 0x00, 0x00, 0x00,   0x02, 0x00, 0x00, 0x00,   0x16, 0x00, 0x03, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x20, 0x00, 0x00, 0x00,   0x17, 0x00, 0x04, 0x00,   0x07, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,   0x08, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,
            0x07, 0x00, 0x00, 0x00,   0x3b, 0x00, 0x04, 0x00,   0x08, 0x00, 0x00, 0x00,   0x09, 0x00, 0x00, 0x00,
            0x03, 0x00, 0x00, 0x00,   0x17, 0x00, 0x04, 0x00,   0x0a, 0x00, 0x00, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x03, 0x00, 0x00, 0x00,   0x20, 0x00, 0x04, 0x00,   0x0b, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,
            0x0a, 0x00, 0x00, 0x00,   0x3b, 0x00, 0x04, 0x00,   0x0b, 0x00, 0x00, 0x00,   0x0c, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00,   0x2b, 0x00, 0x04, 0x00,   0x06, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x80, 0x3f,   0x36, 0x00, 0x05, 0x00,   0x02, 0x00, 0x00, 0x00,   0x04, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,   0x03, 0x00, 0x00, 0x00,   0xf8, 0x00, 0x02, 0x00,   0x05, 0x00, 0x00, 0x00,
            0x3d, 0x00, 0x04, 0x00,   0x0a, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,   0x0c, 0x00, 0x00, 0x00,
            0x51, 0x00, 0x05, 0x00,   0x06, 0x00, 0x00, 0x00,   0x0f, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,   0x51, 0x00, 0x05, 0x00,   0x06, 0x00, 0x00, 0x00,   0x10, 0x00, 0x00, 0x00,
            0x0d, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x51, 0x00, 0x05, 0x00,   0x06, 0x00, 0x00, 0x00,
            0x11, 0x00, 0x00, 0x00,   0x0d, 0x00, 0x00, 0x00,   0x02, 0x00, 0x00, 0x00,   0x50, 0x00, 0x07, 0x00,
            0x07, 0x00, 0x00, 0x00,   0x12, 0x00, 0x00, 0x00,   0x0f, 0x00, 0x00, 0x00,   0x10, 0x00, 0x00, 0x00,
            0x11, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x00, 0x00,   0x3e, 0x00, 0x03, 0x00,   0x09, 0x00, 0x00, 0x00,
            0x12, 0x00, 0x00, 0x00,   0xfd, 0x00, 0x01, 0x00,   0x38, 0x00, 0x01, 0x00,
    };
}

namespace VX::Core {

    class GraphicsDelegateImpl: public Graphics::GraphicsDelegate {
    public:
        void handle_stage(const Graphics::GraphicsStage& stage, Graphics::GraphicsContext& context) const override
        {

        }
    };

    VX::Expected<RenderPipeline> RenderPipelineBuilder::build(Graphics::Instance &instance) const {
        Graphics::GraphicsPipelineBuilder builder;
        for (const auto& entry : m_entries) {
            Graphics::GraphicsStageInfo stage_info = {
                    .user_data = { },
                    .render_target = entry.render_target().handle(),
            };

            builder.add_stage(stage_info);
        }

        return instance.create_graphics_pipeline(builder).map([&](Graphics::GraphicsPipelineHandle pipeline_handle) {
           return RenderPipeline(pipeline_handle);
        });
    }

    RenderTarget RenderAllocator::create_render_target(const RenderTarget::BackingStore &backing) {
        Graphics::RenderTargetHandle handle = std::visit(
            VX::Overloaded {
                [&](const RenderTarget::BackingStore::SwapchainBacked&) {
                    return m_instance->swapchain_info().render_target();
                },
                [&](const RenderTarget::BackingStore::BufferBacked&) {
                    return m_instance->swapchain_info().render_target();
                },
            },
            backing.descriptor()
        );

        return RenderTarget(Graphics::to_shared_handle(m_instance, handle));
    }

    void Renderer::add_hook(std::shared_ptr<RenderHook> hook)
    {
        m_hooks.push_back(std::move(hook));
        invalidate();
    }

    VX::Expected<RenderPipeline> Renderer::reconstruct()
    {
        // TODO: This doesn't go here
        RenderAllocator allocator(m_instance);
        RenderTarget target = allocator.create_render_target(RenderTarget::BackingStore::swapchain_buffer());

        RenderPipelineBuilder builder;
        builder.add_render_stage(target, { }, 0);

        return builder.build(*m_instance);
    }

    void Renderer::render_frame()
    {
        if (!m_state.render_pipeline) {
            auto maybe_pipeline = reconstruct();
            if (maybe_pipeline) {
                m_state.render_pipeline = std::move(maybe_pipeline.value());
            } else {
                if (!m_state.had_construct_failure_last_frame)
                    Log::error("Unable to create graphics pipeline: {}", maybe_pipeline.error().description());

                m_state.had_construct_failure_last_frame = true;
                return;
            }

            m_state.had_construct_failure_last_frame = false;
        }

        GraphicsDelegateImpl delegate;
        m_instance->execute_graphics_pipeline(m_state.render_pipeline->graphics_pipeline(), delegate);
    }
}
