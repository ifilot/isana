#version 330 core

in  vec3 position0;
in  vec2 texcoord0;
in  vec3 position_worldspace;
in  vec3 eye_cameraspace;
in  vec3 lightdirection_cameraspace;
in  vec3 normal_cameraspace;

out vec4 fragColor;

uniform vec4 ambient_light;
uniform sampler2D tex;

void main() {
    // light source
    vec3 l = normalize(lightdirection_cameraspace);

    // normal
    vec3 n = normalize(normal_cameraspace);

    // eye position
    vec3 e = normalize(eye_cameraspace);
    vec3 r = reflect(-l, n);

    // calculate diffuse
    float cosTheta = clamp(dot(n, l), 0, 1);

    // calculate reflection
    float cosAlpha = clamp(dot(e,r), 0, 1);

    vec3 light_color = vec3(ambient_light);
    float lightpower = 1.5f;

    // set vertex color
    vec3 color = vec3(0.8f, 0.8f, 0.8f); //texture(tex, texcoord0.st).rgb;;

    vec3 base = color * 0.2;
    vec3 ambient = color * light_color * 0.4;
    vec3 diffuse = color * lightpower * cosTheta * 0.2;
    vec3 specular = vec3(1,1,1) * lightpower * pow(cosAlpha, 5);

    fragColor = vec4(base + ambient + diffuse + specular, 1.0);
}
