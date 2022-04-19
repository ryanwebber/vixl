#include <VX/Entry/Main.h>
#include <VX/Core/Init.h>

int main(int argc, const char *argv[]) {
    VX::Core::init();
    VX::Entry::Context context = {
        .args = {
            .argc = argc,
            .argv = argv,
        }
    };

    vixl_main(context);
}
