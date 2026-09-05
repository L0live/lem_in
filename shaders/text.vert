#version 330 core

layout (location = 0) in vec4 aPos;
out vec2 TexCoords;
uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(aPos.xy, 1.0, 1.0);
    TexCoords = aPos.zw;
}
