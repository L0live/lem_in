#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 MVP;
uniform vec2 antPosition;
uniform vec2 antDirection;

void main() {
    vec2 finalPosition = aPos + antPosition;
    gl_Position = MVP * vec4(finalPosition, 0.0, 1.0);

    ourColor = aColor;

    vec2 dir = antDirection;
    float len = length(dir);
    if (len > 0.0001)
        dir /= len;
    else
        dir = vec2(1.0, 0.0);

    float dirX = dir.y;
    float dirY = -dir.x;
    dir = vec2(dirX, dirY);

    float c = dir.x;
    float s = dir.y;
    mat2 rot = mat2(c, -s, s, c);

    vec2 centered = aTexCoord - vec2(0.5);
    TexCoord = rot * centered + vec2(0.5);
}
