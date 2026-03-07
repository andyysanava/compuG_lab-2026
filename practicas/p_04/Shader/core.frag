#version 330 core
// in vec3 ourColor;
uniform vec3 ourColor;

out vec4 color;
// uniform vec3 cubeColor;

void main()
{
	// color = vec4(ourColor, 1.0f);
  color = vec4(ourColor, 1.0f);
  // FragColor = vec4(cubeColor, 1.0f);
}
