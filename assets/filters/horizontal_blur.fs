#version 330 core

in vec2 texcoord;
in vec2 blurcoord[14];

out vec4 color;

uniform sampler2D text;

void main() {
    color = vec4(0.0);
    color += texture(text, blurcoord[ 0])*0.0044299121055113265;
    color += texture(text, blurcoord[ 1])*0.00895781211794;
    color += texture(text, blurcoord[ 2])*0.0215963866053;
    color += texture(text, blurcoord[ 3])*0.0443683338718;
    color += texture(text, blurcoord[ 4])*0.0776744219933;
    color += texture(text, blurcoord[ 5])*0.115876621105;
    color += texture(text, blurcoord[ 6])*0.147308056121;
    color += texture(text, texcoord     )*0.159576912161;
    color += texture(text, blurcoord[ 7])*0.147308056121;
    color += texture(text, blurcoord[ 8])*0.115876621105;
    color += texture(text, blurcoord[ 9])*0.0776744219933;
    color += texture(text, blurcoord[10])*0.0443683338718;
    color += texture(text, blurcoord[11])*0.0215963866053;
    color += texture(text, blurcoord[12])*0.00895781211794;
    color += texture(text, blurcoord[13])*0.0044299121055113265;
}
