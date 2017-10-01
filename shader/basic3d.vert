#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

layout(location = 0) in vec4 position;

layout(location = 1) in vec4 color;
layout(location = 2) out vec4 fragColor;

void main() {
	fragColor = color;
    gl_Position = position;
}