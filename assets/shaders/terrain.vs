#version 330 core

in vec3 position;
in vec3 normal;
in vec3 color;

out vec3 position0;
out vec3 color0;

out vec3 position_worldspace;
out vec3 eye_cameraspace;
out vec3 lightdirection_cameraspace;
out vec3 normal_cameraspace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;

void main() {
    // output position of the vertex
    gl_Position = mvp * vec4(position, 1.0);
    position0 = position;
    color0 = color;

    //position of the vertex in world_space
    position_worldspace = (model * vec4(position, 1.0)).xyz;

    vec3 position_cameraspace = (view * model * vec4(position, 1.0)).xyz;
    eye_cameraspace = vec3(0,0,0) - position_cameraspace;

    vec3 light_direction_worldspace = vec3(0, 0, -1);
    lightdirection_cameraspace = (view * vec4(light_direction_worldspace, 0)).xyz;

    normal_cameraspace = (view * model * vec4(normal, 0)).xyz;
}
