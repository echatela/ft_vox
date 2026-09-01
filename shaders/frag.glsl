#version 460 core
flat in int vFace;

out vec4 FragColor;

const vec3 kFaceColor[6] = {
    vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f,0.0f), vec3(1.0f,1.0f,1.0f),
    vec3(1.0f,1.0f,0.0f), vec3(1.0f,0.0f,0.0f), vec3(1.0f,0.5f,0.0f)
};

uniform sampler2D myTexture;

void main()
{
	FragColor = vec4(kFaceColor[vFace], 1.0f);
}
