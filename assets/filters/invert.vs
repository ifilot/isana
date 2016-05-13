#version 330 core

in vec2 position;

out vec2 texcoord;

uniform sampler2D text;

void main() {
    gl_Position = vec4(position, 0.01, 1.0);
    texcoord = (position + 1.0) / 2.0;
}
