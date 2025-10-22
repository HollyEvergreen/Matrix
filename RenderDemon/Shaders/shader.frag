#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
    vec2 uv = fragColor.xy;
    vec3 col = vec3(sin(uv.x), sin(uv.y), 0.);

    outColor = vec4(col, 1.0);
}
