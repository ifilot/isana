#version 330 core

in vec2 texcoord;
out vec4 color;

uniform sampler2D text;
uniform vec3 textcolor;

void main() {
    float ra = texture(text, texcoord).r;
    const float delta = 0.4f;

    if(ra > 0.5f - delta) {
        ra = 1.0f;
    }
    ra = smoothstep(0.5f - delta, 0.5f + delta, ra);

    color = vec4(textcolor, ra);
}
