#version 330 core
layout(location = 0) in vec3 aPos;

//out vec3 pos2color;

out vec4 vertexColor;

// eltolas ennyivel
//uniform float horizontal_offset;

void main()
{
    // eredeti
    gl_Position = vec4(aPos, 1.0);

    // fuggoleges tukrozes
    //gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);

    // vizszintes eltolas
    //gl_Position = vec4(aPos.x + horizontal_offset, aPos.y, aPos.z, 1.0);

    vertexColor = vec4(1.0, 1.0, 1.0, 1.0);

    // ha a pozicio szerint allitjuk be a szint
    //pos2color = aPos;
}
