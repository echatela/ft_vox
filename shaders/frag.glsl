#version 460 core

out vec4 FragColor;

uniform sampler2DArray uBlocks;
flat in int vLayer;
in vec2 vUV;

void main()
{
    FragColor = texture(uBlocks, vec3(vUV, float(vLayer)));
}
