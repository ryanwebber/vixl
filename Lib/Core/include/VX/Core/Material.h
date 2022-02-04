#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <VX/Copyable.h>

#include <VX/Core/Handle.h>
#include <VX/Core/Shader.h>
#include <VX/Core/Texture.h>
#include <VX/Core/Types.h>

namespace VX::Core {

    struct SampleTexture {
        std::shared_ptr<UniformHandle> sampler_handle;
        std::shared_ptr<TextureHandle> texture_handle;
    };

    class Material final {
        VX_DEFAULT_MOVABLE(Material);
    private:
        std::string m_name;
        std::shared_ptr<ProgramHandle> m_shader_program;
        std::shared_ptr<ShaderHandle> m_vertex_shader;
        std::shared_ptr<ShaderHandle> m_fragment_shader;

        std::unordered_map<uint8_t, SampleTexture> m_texture_slots;

        // Private copy constructor
        Material(const Material&) noexcept = default;
        Material& operator=(const Material&) noexcept = default;

    public:
        Material(const std::string_view& name, std::shared_ptr<ShaderHandle> vertex_shader, std::shared_ptr<ShaderHandle> fragment_shader);
        Material(const std::string_view &name, const uint8_t* vertex_data, size_t vertex_data_len, const uint8_t* fragment_data, size_t fragment_data_len);
        ~Material() = default;

        [[nodiscard]] Material clone() const;

        [[nodiscard]] ShaderHandle vertex_shader_handle() const { return *m_vertex_shader; }
        [[nodiscard]] ShaderHandle fragment_shader_handle() const { return *m_fragment_shader; }
        [[nodiscard]] ProgramHandle shader_program_handle() const { return *m_shader_program; }

        [[nodiscard]] const std::unordered_map<uint8_t, SampleTexture>& texture_slots() const { return m_texture_slots; }

        [[nodiscard]] bool is_valid() const {
            return m_shader_program != nullptr && m_shader_program->is_valid();
        }

        template <uint8_t SLOT>
        void set_texture(const SampleTexture &sample_texture) {
            m_texture_slots[SLOT] = sample_texture;
        }

        template <uint8_t SLOT>
        void set_texture(const std::string sampler_name, const Texture &texture) {
#warning "Material#set_texture not implemented"
        }
    };
}
