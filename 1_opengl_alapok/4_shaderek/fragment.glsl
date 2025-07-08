#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
//in vec3 pos2color;

void main()
{
    //FragColor = vec4(pos2color, 1.0);
    FragColor = vertexColor;
}
