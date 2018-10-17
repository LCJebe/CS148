#version 330

uniform float inputTime;

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main()
{
    fragmentColor = vertexColor;
    vec4 modifiedVertexPosition = vertexPosition;

    // Insert your code for "Slightly-More Advanced Shaders" here.

    // create triangle signal
    float a = 1.0f;
    float b = 0.0f;
    float sig = inputTime - floor(inputTime / 2.0f / a)*2.0f*a + b;

    modifiedVertexPosition.y -= sig < b+a ? sig : -sig + 2.0f*a + 2.0f*b;

    gl_Position = modifiedVertexPosition;
}
