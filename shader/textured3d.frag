#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 1) in vec2 uvCoords;
layout(location = 2) uniform sampler2D texture;


void main() {
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}