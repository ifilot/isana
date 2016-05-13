#version 330 core

uniform sampler2D text;

in vec2 texcoord;

out vec4 color;

void main() {
    color = texture(text, texcoord);
}
