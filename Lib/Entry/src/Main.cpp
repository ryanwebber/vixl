#include <VX/Entry/Main.h>

int main(int argc, const char *argv[]) {
    VX::Entry::Context context = {
        .args = {
            .argc = argc,
            .argv = argv,
        }
    };

    vixl_main(context);
}
