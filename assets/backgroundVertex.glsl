#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 texCoord;

void main() {
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    if(aPos.x < 0.0f) {
        texCoord.x = 0.0f;
    } else {
        texCoord.x = 1.0f;
    }

    if(aPos.y < 0.0f) {
        texCoord.y = 1.0f;
    } else {
        texCoord.y = 0.0f;
    }
}
