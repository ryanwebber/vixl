$input a_position
$output v_color0

#include <bgfx_shader.sh>

void main()
{
	gl_Position = vec4(a_position, 0.0f);
	v_color0 = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
