#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 MVP;
uniform vec2 antPosition;

void main() {
    vec2 finalPosition = aPos + antPosition;
    gl_Position = MVP * vec4(finalPosition, 0.0, 1.0);

    ourColor = aColor;
    TexCoord = aTexCoord;
}
