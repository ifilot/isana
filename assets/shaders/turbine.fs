#version 330 core

in  vec3 position0;
in  vec2 texcoord0;
in  vec3 position_worldspace;
in  vec3 eye_cameraspace;
in  vec3 lightdirection_cameraspace;
in  vec3 normal_cameraspace;

out vec4 fragColor;

uniform vec4 ambient_color;
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

    vec3 light_color = vec3(1,1,1);
    float lightpower = 1.0;

    float z = clamp((position0.z + 5.0f) / 10.0, 0, 1);
    vec3 color = texture(tex, texcoord0.st).rgb;
    vec3 environment_light = ambient_color.rgb;
    vec3 ambient = color;
    vec3 diffuse = color * lightpower * cosTheta;
    vec3 specular = vec3(1,1,1) * light_color * lightpower * pow(cosAlpha, 5);

    fragColor = vec4(0.2 * environment_light +
                     0.2 * ambient +
                     0.4 * diffuse +
                     0.2 * specular, 1.0);
}
