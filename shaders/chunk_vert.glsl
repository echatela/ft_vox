#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in int aFace;
layout (location = 2) in int aCorner;
layout (location = 3) in int aId;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
flat out int id;

const vec3 kCorners[6][4] = {
    {vec3(1, 0, 1), vec3(1, 0, 0), vec3(1, 1, 0), vec3(1, 1, 1)}, // +X
    {vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 1, 1), vec3(0, 1, 0)}, // -X
    {vec3(0, 1, 1), vec3(1, 1, 1), vec3(1, 1, 0), vec3(0, 1, 0)}, // +Y
    {vec3(0, 0, 0), vec3(1, 0, 0), vec3(1, 0, 1), vec3(0, 0, 1)}, // -Y
    {vec3(0, 0, 1), vec3(1, 0, 1), vec3(1, 1, 1), vec3(0, 1, 1)}, // +Z
    {vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(1, 1, 0)}, // -Z
};
const vec2 kTexCoord[4] = {vec2(0,0),vec2(1,0),vec2(1,1),vec2(0,1)};

void main()
{
	gl_Position = projection * view * model *
        vec4(aPos + kCorners[aFace][aCorner], 1.0f);
    TexCoord = kTexCoord[aCorner];
    id = aId;
}
