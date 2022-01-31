#pragma once

#include <vector>

namespace VX::Entry {
    struct Context {
        struct {
            int argc;
            const char **argv;
        } args { };
    };
}

extern int vixl_main(const VX::Entry::Context &ctx);
