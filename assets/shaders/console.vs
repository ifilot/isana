#version 330 core

in vec3 position;
out vec2 texcoord;

void main() {
    gl_Position = vec4(position.xy, 0.02, 1.0);
    texcoord = (position.xy + 1.0) / 2.0;
}
