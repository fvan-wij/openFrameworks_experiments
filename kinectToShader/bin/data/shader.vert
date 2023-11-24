#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

uniform float time;

uniform float mouseX;
uniform float mouseY;
uniform vec2 mousePos;
uniform float range;

uniform vec2 hands[16];

void main()
{
	vec4 pos;
	vec2 dir;
	vec2 finalDisplacement = vec2(0.0, 0.0);

	for (int i = 0; i < 16; i++)
	{
		pos = position;
		dir = pos.xy - hands[i];

		float dist = distance(pos.xy, hands[i]);

		if (hands[i] != 0.0 && dist > 0.0 && dist < range)
		{
			float distNorm = dist / range;
			distNorm = 1.0 - distNorm;
			dir *= distNorm;
			pos.x += dir.x;
			pos.y += dir.y;
			finalDisplacement += dir;
		}
	}
	pos.x += finalDisplacement.x;
	pos.y += finalDisplacement.y;

	gl_Position = modelViewProjectionMatrix * pos;
}
