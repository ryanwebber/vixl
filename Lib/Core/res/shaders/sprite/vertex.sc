$input a_position, a_texcoord0
$output v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor,  0);

void main()
{
    vec2 texture_size = bgfxTextureSize(s_texColor, 1);
    vec2 position = vec2(a_position.x, a_position.y);
    // vec2 aspect = vec2(1, 1);
    vec2 aspect = texture_size + vec2(1, 1);
    if (texture_size.x > texture_size.y) {
        // aspect.y = texture_size.y / texture_size.x;
    } else {
        // aspect.x = texture_size.x / texture_size.y;
    }

	gl_Position = mul(u_modelViewProj, vec4(position * aspect, 0.0, 1.0));
	v_texcoord0 = a_texcoord0;
}
