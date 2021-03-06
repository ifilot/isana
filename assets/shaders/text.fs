#version 330 core

in vec2 texcoord;
out vec4 color;

uniform sampler2D text;
uniform vec3 textcolor;

void main() {
    float ra = texture(text, texcoord).r;
    color = vec4(textcolor, ra);
}
