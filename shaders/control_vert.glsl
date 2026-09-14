#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 UV;

uniform vec2 offset;
uniform vec2 rect;
// uniform vec2 modelPos;

void main()
{
	vec2 pos = vec2((aPos.x + offset.x) / rect.x, (aPos.y - offset.y) / rect.y) * 2;
	// pos = pos + vec2(modelPos.x / rect.x, modelPos.y / -rect.y) * 2;
	pos += vec2(-1, 1);
	gl_Position = vec4(pos, -1.f, 1.f);
	UV = aTexCoord;
}
