#version 330 core

uniform mat4 modelViewProjectionMatrix;
// uniform float pos_scaler;
// uniform float midi12;


in vec4 position;
in vec2 texcoord;

out vec2 texcoord1;

void main()
{
    // float pos_scaler = midi12
    texcoord1 = texcoord;
    // vec4 scaled_pos = vec4((pos_scaler * position.xyz), 1.0);
    // gl_Position = modelViewProjectionMatrix * scaled_pos;
        gl_Position = modelViewProjectionMatrix * position;

}
