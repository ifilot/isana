#version 330 core

in vec2 position;

out vec2 texcoord;
out vec2 blurcoord[14];

uniform sampler2D text;

void main() {
    gl_Position = vec4(position, 0.01, 1.0);
    texcoord = (position + 1.0) / 2.0;
    blurcoord[ 0] = texcoord + vec2(-0.028, 0.0);
    blurcoord[ 1] = texcoord + vec2(-0.024, 0.0);
    blurcoord[ 2] = texcoord + vec2(-0.020, 0.0);
    blurcoord[ 3] = texcoord + vec2(-0.016, 0.0);
    blurcoord[ 4] = texcoord + vec2(-0.012, 0.0);
    blurcoord[ 5] = texcoord + vec2(-0.008, 0.0);
    blurcoord[ 6] = texcoord + vec2(-0.004, 0.0);
    blurcoord[ 7] = texcoord + vec2( 0.004, 0.0);
    blurcoord[ 8] = texcoord + vec2( 0.008, 0.0);
    blurcoord[ 9] = texcoord + vec2( 0.012, 0.0);
    blurcoord[10] = texcoord + vec2( 0.016, 0.0);
    blurcoord[11] = texcoord + vec2( 0.020, 0.0);
    blurcoord[12] = texcoord + vec2( 0.024, 0.0);
    blurcoord[13] = texcoord + vec2( 0.028, 0.0);
}
