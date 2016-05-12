#version 330 core

in vec2 texcoord;
out vec4 color;

uniform sampler2D text;
uniform vec3 textcolor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texcoord).r);
    color = vec4(textcolor, 1.0) * sampled;
}
