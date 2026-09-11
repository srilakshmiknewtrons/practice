#version 330 core
in vec3 OutColor;
in vec2 uvTex;
out vec4 FragColor;

uniform sampler2D sampler;
uniform  bool isUseTexture;
void main()
{
    if(isUseTexture)
    {
        FragColor = texture(sampler,uvTex);
    }
    else 
    {
        FragColor = vec4(OutColor,1.0);

    }
    

}