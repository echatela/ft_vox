#version 460 core

out vec4 FragColor;

uniform sampler2DArray uBlocksTexture;
flat in int vLayer;
in vec2 vUV;

void main()
{
    FragColor = texture(uBlocksTexture, vec3(vUV, float(vLayer)));
}
