#version 330 core

in vec2 texcoord;
out vec4 color;

uniform sampler2D text;
uniform vec3 textcolor;

float width = 0.46;
float edge = 0.19;

void main() {
    float distance = 1.0 - texture(text, texcoord).r;
    float alpha = 1.0 - smoothstep(width, width+edge, distance);

    color = vec4(textcolor, alpha);
}
