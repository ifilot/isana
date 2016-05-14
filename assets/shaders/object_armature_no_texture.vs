#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texture_coordinate;
in vec4 weights;

out vec3 position_worldspace;
out vec3 eye_cameraspace;
out vec3 lightdirection_cameraspace;
out vec3 normal_cameraspace;
out vec3 position0;
out vec2 texcoord0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
uniform mat4 armature[3];

void main() {
    // output position of the vertex
    vec4 pos = vec4(0,0,0,1);
    vec4 nor = vec4(0,0,0,1);

    for(int i=0; i<3; i++) {
        pos += weights[i] * vec4(position, 1.0) * armature[i];
        nor += weights[i] * vec4(normal, 0.0) * armature[i];
    }

    vec4 new_pos =vec4(pos.xyz, 1.0);

    gl_Position = mvp * new_pos;
    position0 = new_pos.xyz;
    texcoord0 = texture_coordinate;

    //position of the vertex in world_space
    position_worldspace = (model * new_pos).xyz;

    vec3 position_cameraspace = (view * model * new_pos).xyz;
    eye_cameraspace = vec3(0,0,0) - position_cameraspace;

    vec3 light_direction_worldspace = vec3(0, 0, -1);
    lightdirection_cameraspace = (view * vec4(light_direction_worldspace, 0)).xyz;

    normal_cameraspace = (view * model * nor).xyz;
}
