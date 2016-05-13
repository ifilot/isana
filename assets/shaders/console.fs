#version 330 core

uniform vec4 back_color;

in vec2 texcoord;

out vec4 color;

void main() {
    color = back_color;
}
