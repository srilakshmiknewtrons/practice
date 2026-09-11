#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;
 
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool isUseMatrix; 
out vec3 OutColor;
out vec2 uvTex;
void main()
{
    if(isUseMatrix)
    {
        gl_Position = projection * view *  model * vec4(position,1.0);
    }
    else
    {
        gl_Position = vec4(position,1.0);
    }

    
    OutColor = color;
    uvTex = uv;

}