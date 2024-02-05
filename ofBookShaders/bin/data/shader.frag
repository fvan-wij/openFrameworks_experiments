#version 150

out vec4 outputColor;
uniform float mouseX;
uniform float mouseY;
uniform vec4 globalColor;

void main()
{
    outputColor = globalColor;
}
