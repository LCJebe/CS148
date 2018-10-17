#version 330

in vec3 fragmentColor;

out vec3 fragColor;

uniform float inputTime;

void main() 
{
    vec3 finalColor = vec3(1, 0, 0);

    // Insert your code for "Slightly-More Advanced Shaders" here.
    finalColor = fragmentColor;

    /*
    // create triangle signal (here used for coloring)
    float a = 2.0f;
    float b = 0.0f;

    float t1 = inputTime;
    float t2 = inputTime + a*1.0f/3.0f;
    float t3 = inputTime + a*2.0f/3.0f;

    float sig1 = t1 - floor(t1 / 2.0f / a)*2.0f*a + b;
    float sig2 = t2 - floor(t2 / 2.0f / a)*2.0f*a + b;
    float sig3 = t3 - floor(t3 / 2.0f / a)*2.0f*a + b;

    sig1 = sig1 < b+a ? sig1 : -sig1 + 2.0f*a + 2.0f*b;
    sig2 = sig2 < b+a ? sig2 : -sig2 + 2.0f*a + 2.0f*b;
    sig3 = sig3 < b+a ? sig3 : -sig3 + 2.0f*a + 2.0f*b;

    finalColor = vec3(sig1 / a, sig2 / a, sig3 / a);
    */

    fragColor = finalColor;
}
