#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

uniform float time;

uniform float mouseX;
uniform float mouseY;
uniform vec2 mousePos;
uniform float range;

void main()
{
	vec4 pos = position;
	vec2 dir = position.xy - mousePos;

	float dist = distance(pos.xy, mousePos);

	if (dist > 0.0 && dist < range)
	{
		float distNorm = dist / range;
		distNorm = 1.0 - distNorm;
		dir *= distNorm;
		pos.x += dir.x;
		pos.y += dir.y;
	}

	gl_Position = modelViewProjectionMatrix * pos;
}
