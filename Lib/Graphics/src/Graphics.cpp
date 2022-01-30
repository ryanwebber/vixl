#include <iostream>

#include <vulkan/vulkan.h>
#include <VX/Graphics/Graphics.h>

void VX::Graphics::say_hello() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::cout << extensionCount << " extensions supported\n";
}
